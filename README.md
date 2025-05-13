
  <h1>Minishell</h1>
  <p><strong>Minishell</strong> is a basic Unix shell built in <strong>C</strong> that mimics core behaviors of Bash. It is part of the 42 curriculum and introduces students to process management, input/output redirection, and signal handling by reimplementing a miniature shell environment.</p>

  <h2>Features</h2>
  <ul>
    <li>Interactive prompt with command history</li>
    <li>Execution of system commands via <code>PATH</code></li>
    <li>Support for absolute and relative paths</li>
    <li>Environment variable expansion (e.g., <code>$HOME</code>)</li>
    <li>Redirections: <code>&lt;</code>, <code>&gt;</code>, <code>&lt;&lt;</code> (heredoc), <code>&gt;&gt;</code></li>
    <li>Pipes to chain commands (e.g., <code>ls | grep .c</code>)</li>
    <li>Built-in commands: <code>cd</code>, <code>echo</code>, <code>pwd</code>, <code>env</code>, <code>export</code>, <code>unset</code>, <code>exit</code></li>
    <li>Proper handling of <code>ctrl-C</code>, <code>ctrl-D</code>, and <code>ctrl-\</code></li>
  </ul>

  <h2>Requirements</h2>
  <ul>
    <li>GCC compiler</li>
    <li>Make utility</li>
    <li>Readline library (for prompt and history)</li>
  </ul>

  <h2>Compilation</h2>
  <pre><code>make</code></pre>

  <h2>Usage</h2>
  <pre><code>./minishell</code></pre>
  <p>Launches the shell. Type commands as you would in Bash.</p>

  <h2>Controls</h2>
  <ul>
    <li><strong>ctrl-C</strong> – Interrupt the current process and display a new prompt</li>
    <li><strong>ctrl-D</strong> – Exit the shell</li>
    <li><strong>ctrl-\</strong> – Ignored (like Bash)</li>
  </ul>

  <h2>Supported Built-ins</h2>
  <ul>
    <li><code>echo [-n]</code></li>
    <li><code>cd [dir]</code></li>
    <li><code>pwd</code></li>
    <li><code>export [VAR=value]</code></li>
    <li><code>unset [VAR]</code></li>
    <li><code>env</code></li>
    <li><code>exit</code></li>
  </ul>
