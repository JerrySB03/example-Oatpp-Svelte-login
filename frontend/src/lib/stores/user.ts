export interface UserLogin {
	username: string;
	password: string;
}
export interface UserSignUp{
    username: string;
    email: string;
    password: string;
}
export interface UserData{
    id: string;
    username: string;
    email: string;
    password: null;
}
export interface AuthData{
    token: string;
}