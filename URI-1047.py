a,b,c,d = map(int,input().split())    
start = a*60 + b
end_ = c*60 + d
duracao = 0


if(a <= c):
    duracao = end_ - start
    if(duracao == 0):
        print("O JOGO DUROU %d HORA(S) E %d MINUTO(S)" %(24,duracao%60))
    elif(duracao<0):
        if(b > d):
            hr = 24-a+c-1
        else:
            hr = 24-a+c
        print("O JOGO DUROU %d HORA(S) E %d MINUTO(S)" %(hr,duracao%60))
    else:
        print("O JOGO DUROU %d HORA(S) E %d MINUTO(S)" %((duracao - duracao%60)/60,duracao%60)) 
else:
    duracao = (24*60 - start) + end_
    print("O JOGO DUROU %d HORA(S) E %d MINUTO(S)" %((duracao - duracao%60)/60,duracao%60))