<script>
    let username = '';
    let password = '';
  
    function handleLogin() {
      // Handle login functionality here
      console.log('Logging in...');
    }
  </script>
  
  <main>
    <h1>Login</h1>
    <form on:submit|preventDefault={handleLogin}>
      <label for="username">Username</label>
      <input type="text" id="username" bind:value={username}>
  
      <label for="password">Password</label>
      <input type="password" id="password" bind:value={password}>
  
      <button type="submit">Log In</button>
    </form>
  </main>
  
  <style>
    main {
      max-width: 400px;
      margin: 0 auto;
      padding: 40px;
      background-color: #FFF;
      border-radius: 5px;
      box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
    }
  
    h1 {
      text-align: center;
      color: #001C30;
    }
  
    label {
      display: block;
      margin-bottom: 10px;
      color: #176B87;
    }
  
    input[type="text"],
    input[type="password"] {
      width: 100%;
      padding: 10px;
      border: 1px solid #64CCC5;
      border-radius: 5px;
      margin-bottom: 20px;
    }
  
    button {
      display: block;
      width: 100%;
      padding: 10px;
      background-color: #176B87;
      border: none;
      border-radius: 5px;
      color: #FFF;
      font-size: 16px;
      cursor: pointer;
    }
  
    button:hover {
      background-color: #001C30;
    }
  </style>
  