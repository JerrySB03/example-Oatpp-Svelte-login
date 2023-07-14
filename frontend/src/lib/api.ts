import type { AuthData, UserData, UserLogin, UserSignUp } from "$lib/stores/user";
import type { StatusData } from "$lib/stores/error";

export async function SignUp(data: UserSignUp): Promise<AuthData> {
    const res = await fetch("/api/users/signup", {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
    });
    if (res.status === 200) {
        return res.json();
    } else if (res.status === 409) { 
        const data: StatusData = await res.json();
        throw new Error(data.message);
    }
    else {
        throw new Error("Sign up failed");
    }
}
export async function Login(data:UserLogin) {
    const res = await fetch("/api/users/login", {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
    });
    if (res.status === 200) {
        return res.json();
    }
    else if (res.status === 401) {
        const data: StatusData = await res.json();
        throw new Error(data.message);
    }
}