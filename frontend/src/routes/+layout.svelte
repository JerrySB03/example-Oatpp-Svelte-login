<script>
    let primaryColor = "#DAFFFB";

    // Navigation links
    const navLinks = [
        { text: "Home", href: "/" },
        { text: "Login", href: "/login" },
        { text: "Signup", href: "/login/signup" },
        { text: "Swagger", href: "/swagger/ui" },
        { text: "GitHub", href: "https://github.com" },
    ];
</script>

<div class="navbar">
    {#each navLinks as link}
        {#if link.text === "GitHub"}
            <a href={link.href} target="_blank" rel="noopener noreferrer">
                <img
                    src="/assets/github-logo.png"
                    alt="GitHub"
                    class="github-logo"
                />
            </a>
        {:else}
            <a href={link.href}>{link.text}</a>
        {/if}
    {/each}
</div>
<slot />

<style>
    .navbar {
        background-color: #3B5A67;
        display: flex;
        justify-content: space-between;
        padding: 1rem;
        color: #001c30;
        border-radius: 1rem;
    }

    .navbar a {
        color: inherit;
        text-decoration: none;
        margin: 0 1rem;
    }

    .navbar a:hover {
        color: #64ccc5;
    }

    .navbar .active {
        color: #176b87;
    }
    
  .github-logo {
    height: 1.5rem; 
    width: auto; 
    margin-left: 1rem; 
    vertical-align: middle;
  }
</style>
