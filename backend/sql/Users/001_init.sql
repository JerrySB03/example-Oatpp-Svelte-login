CREATE EXTENSION IF NOT EXISTS "uuid-ossp";
CREATE TABLE IF NOT EXISTS Users (
    id UUID NOT NULL DEFAULT uuid_generate_v4() PRIMARY KEY,
    username VARCHAR(127) UNIQUE NOT NULL,
    email VARCHAR(255) UNIQUE NOT NULL,
    hash TEXT NOT NULL
);
INSERT INTO Users (username, email, hash)
VALUES (
        'test',
        'example@example.com',
        '$argon2id$v=19$m=4096,t=1,p=1$w4DCq8K6w6XCmxJqw4nDrcK+wqNfw5ETw77DnQ/Ct8O/w4JRKAxBczQHw6zCr8OpQsKB$/K4piV/osE0CVESjKFk8G+ce8NJoklex2GWSjJS3CkGSWEu27Y5b4Nzs49cwBwLlcDWSLWfi30c1K4v1V+Zw5Q'
    );
--Password is 'password'
INSERT INTO Users (username, email, hash)
VALUES (
        'test2',
        'example2@example.com',
        '$argon2id$v=19$m=4096,t=1,p=1$E8KVVMKWfcODbWx9w5tLwpjCnMOMcMKvw7DDpcKJAA7DksOIY8Ouwr5nwoN8wocAwpY$k0qhFScX5JsnmEqBR3QDGHDlGZbSlca+G4fPR28zKuIenyh0MQBZ1c4C0bstKsoxpyIfc0UFiFPlY7/cAxEdjw'
    );
--Password is also 'password'