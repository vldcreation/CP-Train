class User{
    constructor(username){
        this.username = username;
    }
    getUsername(){
        return this.username;
    }
    setUsername(username){
        this.username = username;
    }
}

class ChatUser extends User{
    constructor(userName){
        super(userName);
        this.warning_count = 0;
    }
    giveWarning(){
        this.warning_count++;
    }
    getWarningCount(){
        return this.warning_count;
    }

}