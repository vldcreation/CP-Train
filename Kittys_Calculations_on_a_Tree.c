// https://www.hackerrank.com/challenges/kittys-calculations-on-a-tree/problem

// Coded by : sammyon7

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

static int timeme;

static inline void timespec_minus(struct timespec *a,
                                  const struct timespec *b) {
    a->tv_sec -= b->tv_sec;
    if (b->tv_nsec > a->tv_nsec) {
        a->tv_sec--;
        a->tv_nsec += 1e9;
    }
    a->tv_nsec -= b->tv_nsec;
}

// *a += *b
static inline void timespec_accumulate(struct timespec *a,
                                       const struct timespec *b) {
    a->tv_sec += b->tv_sec;
    a->tv_nsec += b->tv_nsec;
    if (a->tv_nsec >= 1e9) {
        a->tv_nsec -= 1e9;
        a->tv_sec++;
    }
}

static size_t bytesMallocd;

static void *mallocOrDie(size_t bytes) {
    void *p = malloc(bytes);
    assert(p != NULL);
    bytesMallocd += bytes;
    return p;
}

typedef int64_t SumType;
typedef int32_t NodeType;

typedef struct Node {
    NodeType parent, depth, next;
} Node;

typedef struct NodeQueryInfo {
    SumType diqSum;
    SumType partial;
    SumType sumFrom;
} NodeQueryInfo;

typedef struct NodeSet {
    int bytes;
    uint8_t *present;
} NodeSet;

static NodeSet *newNodeSet(NodeType maxVal) {
    NodeSet *nodeset = mallocOrDie(sizeof(NodeSet));
    nodeset->bytes = (maxVal + 8) >> 3;
    nodeset->present = mallocOrDie(nodeset->bytes);
    bzero(nodeset->present, nodeset->bytes);
    return nodeset;
}

static bool contains(const NodeSet *nodeset, NodeType node) {
    return (nodeset->present[node >> 3] & (1 << (node & 7))) != 0;
}

static void add(NodeSet *nodeset, NodeType node) {
    nodeset->present[node >> 3] |= (1 << (node & 7));
}

static void clear(NodeSet *nodeset) {
    bzero(nodeset->present, nodeset->bytes);
}

typedef struct Tree {
    int numNodes, maxDepth, maxQueued;
    Node *restrict nodes;
    NodeSet *restrict enqueued;
    NodeType *restrict queued;
    NodeQueryInfo *restrict qinfo;
} Tree;

static Tree *newTree(int numNodes) {
    Tree *tree = mallocOrDie(sizeof(Tree));
    tree->numNodes = numNodes;
    tree->maxDepth = 0;
    tree->nodes = mallocOrDie(sizeof(Node) * numNodes);
    bzero(tree->nodes, sizeof(Node) * numNodes);
    tree->qinfo = mallocOrDie(sizeof(NodeQueryInfo) * numNodes);
    tree->enqueued = newNodeSet(numNodes);
    return tree;
}

static void addChildTo(Tree *restrict tree, NodeType parent, NodeType child) {
    Node *pnode = &tree->nodes[parent-1];
    Node *cnode = &tree->nodes[child-1];

    cnode->parent = parent;
    cnode->depth = pnode->depth + 1;
    if (cnode->depth > tree->maxDepth) {
        tree->maxDepth = cnode->depth;
    }
}


static Tree *readTree(int n, FILE *reader) {
    int rc;
#ifdef TIMEME
    struct timespec ts0, ts1;
    if (__builtin_expect(timeme, 0)) {
        rc = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts0);
        assert(rc == 0);
    }
#endif
    Tree *tree = newTree(n);
    assert(tree != NULL);
    NodeType a, b;
    for (int i = 0; i < n-1; i++) {
        int rc = fscanf(reader, "%d %d", &a, &b);
        assert(rc == 2);
        if (a == 1 || tree->nodes[a-1].depth > 0) {
                addChildTo(tree, a, b);
        }
        else {
            addChildTo(tree, b, a);
    }
    }
    tree->queued = mallocOrDie(sizeof(NodeType) * (tree->maxDepth + 1));
#ifdef TIMEME
    if (__builtin_expect(timeme, 0)) {
        rc = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts1);
        assert(rc == 0);
        timespec_minus(&ts1, &ts0);
        fprintf(stderr, "readTree took %ld.%09ld seconds\n", ts1.tv_sec, ts1.tv_nsec);
    }
#endif
    return tree;
}

static inline void enqueue(Tree *tree, NodeType n){
    if (! contains(tree->enqueued, n)) {
        Node *node = &tree->nodes[n-1];
        node->next = tree->queued[node->depth];
        tree->queued[node->depth] = n;
        if (node->depth > tree->maxQueued) {
            tree->maxQueued = node->depth;
        }
        add(tree->enqueued, n);
    }
}


static inline NodeType dequeue(Tree *tree) {
    while (tree->maxQueued >= 0) {
        NodeType n = tree->queued[tree->maxQueued];
        if (n != 0) {
            tree->queued[tree->maxQueued] = tree->nodes[n-1].next;
            return n;
        }
        tree->maxQueued -= 1;
    }
    return 0;
}


struct timespec readQueryTime;

static int readQueryInto(Tree *tree, FILE *reader) {
    int rc;
#ifdef TIMEME
    struct timespec ts0, ts1;
    if (__builtin_expect(timeme, 0)) {
        rc = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts0);
        assert(rc == 0);
    }
#endif
    int nn;
    rc = fscanf(reader, "%d", &nn);
    assert(rc == 1);

    clear(tree->enqueued);
    bzero(tree->queued, sizeof(NodeType) * (tree->maxDepth + 1));
    bzero(tree->qinfo, sizeof(NodeQueryInfo) * tree->numNodes);
    tree->maxQueued = 0;

    for (int i = 0; i < nn; i++) {
        NodeType elt;
        rc = fscanf(reader, "%d", &elt);
        assert(rc == 1);
        tree->qinfo[elt-1].diqSum = elt;
        enqueue(tree, elt);
    }
#ifdef TIMEME
    if (__builtin_expect(timeme, 0)) {
        rc = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts1);
        assert(rc == 0);
        timespec_minus(&ts1, &ts0);
        timespec_accumulate(&readQueryTime, &ts1);
    }
#endif
    return nn;
}

static inline SumType modk(SumType sum) {
    const SumType k = 1e9 + 7;
    if (__builtin_expect(sum < k, 1)) {
        return sum;
    }
    return sum % k;
}

static inline void mergeInto(NodeQueryInfo *restrict a, NodeQueryInfo *restrict b) {
    if (__builtin_expect(b->diqSum != 0, 1)) {
        SumType diqSum = modk(a->diqSum + b->diqSum);
        SumType partial = modk(a->partial + b->diqSum + b->partial);
        SumType sumFrom = modk(a->sumFrom + b->sumFrom + modk(a->diqSum * (b->partial + b->diqSum)) + modk(b->diqSum * a->partial));
        a->diqSum = diqSum;
        a->partial = partial;
        a->sumFrom = sumFrom;
    }
}

struct timespec queryTime;

static void dumpInfo(FILE *fp, const Tree *tree) {
    const NodeQueryInfo *info = tree->qinfo;
    for (int n = 1; n <= tree->numNodes; n++) {
        fprintf(fp, "node %d info: %lld %lld %lld\n", n, info[n-1].diqSum, info[n-1].partial, info[n-1].sumFrom);
    }
}

static SumType query(Tree *restrict tree) {
    if (__builtin_expect(tree->numNodes < 2, 0)) {
        return 0;
    }
    int rc;
#ifdef TIMEME
    struct timespec ts0, ts1;
    if (__builtin_expect(timeme, 0)) {
        rc = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts0);
        assert(rc == 0);
    }
#endif
    NodeType n;
    while ((n = dequeue(tree)) != 0) {
        NodeType parent = tree->nodes[n-1].parent;
        if (__builtin_expect(parent > 0, 1)) {
            mergeInto(&tree->qinfo[parent-1], &tree->qinfo[n-1]);
            enqueue(tree, parent);
        }
    }
#ifdef TIMEME
    if (__builtin_expect(timeme, 0)) {
        rc = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts1);
        assert(rc == 0);
        timespec_minus(&ts1, &ts0);
        timespec_accumulate(&queryTime, &ts1);
    }
#endif
    /* dumpInfo(stderr, tree); */
    return tree->qinfo[0].sumFrom;
}


int atoiEnv(const char *name, int dval) {
    char *val = getenv(name);
    if (val == NULL) {
        return dval;
    }
    return atoi(val);
}


int main(int argc, char **argv) {
    int verbosity = atoiEnv("VERBOSITY", 1);
    int rc;

#ifdef TIMEME
    timeme = atoiEnv("TIMEME", 1);
    struct timespec ts0, ts1;
    if (__builtin_expect(timeme, 0)) {
        rc = clock_getres(CLOCK_PROCESS_CPUTIME_ID, &ts0);
        assert(rc == 0);
        fprintf(stderr, "CLOCK_PROCESS_CPUTIME_ID resolution: %ld nanoseconds\n",
                1000000000 * ts0.tv_sec + ts0.tv_nsec);
        rc = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts0);
        assert(rc == 0);
    }
#endif
    FILE *fp = argc < 2 ? stdin : fopen(argv[1], "r");
    assert(fp != NULL);
    int n, q;
    rc = fscanf(fp, "%d %d\n", &n, &q);
    assert(rc == 2);
    Tree *tree = readTree(n, fp);
    if (verbosity > 0) {
        fprintf(stderr, "tree has %d nodes, max depth %d\n", tree->numNodes, tree->maxDepth);
    }
    int totalQueryNodes = 0;
    int j;
    for (int i = 0; i < q; i += 1) {
        int queryLen = readQueryInto(tree, fp);
        if (__builtin_expect(verbosity > 1, 0)) {
            fprintf(stderr, "Submitting query %d (%d nodes)\n", i+1, queryLen);
        }
        totalQueryNodes += queryLen;
        printf("%lld\n", query(tree));
    }
#ifdef TIMEME
    if (__builtin_expect(timeme, 0)) {
        rc = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts1);
        assert(rc == 0);
        timespec_minus(&ts1, &ts0);
        fprintf(stderr, "readQueryTime %ld.%09ld seconds\n", readQueryTime.tv_sec, readQueryTime.tv_nsec);
        fprintf(stderr, "queryTime %ld.%09ld seconds\n", queryTime.tv_sec, queryTime.tv_nsec);
        fprintf(stderr, "TOTAL TIME: %ld.%09ld seconds\n", ts1.tv_sec, ts1.tv_nsec);
    }
#endif
    fprintf(stderr, "%zu total bytes mallocd (%zu MB)\n", bytesMallocd, bytesMallocd >> 20);
    return 0;
}

