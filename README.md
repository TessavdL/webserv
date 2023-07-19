<!DOCTYPE html>
<html><div class="welcome">
  <h1>Hi there, welcome to webserv 👋</h1>
  <subtitle>Webserv is a HTTP webserver that handles static content, form data and image uploading.
  Form data and image uploading is enabled through a Common Gateway Interface that executes php.
  Webserv responds to <code>GET</code>, <code>POST</code> and <code>DELETE</code> HTTP methods.
  Webserv is writting in the C++ programming language and created by vdkJelle and TessavdL.</subtitle>
</div>

<div class="clone">
  <h2>How to clone the repository</h2>
  <ol>
    <li>Above the list of files, click <code>< > Code</code>.</li>
    <li>Copy the url of the repository.</li>
    <li>Open up a terminal.</li>
    <li>Clone the repostiory using the <code>git clone</code> command. You can type in <code>git clone</code> and paste the url you just copied after it. It is also recommended to specify the target location (e.g. where you would like the repository to be cloned to).
<p>

```
git clone <url> <target_location>
```
  </p>
      <p>For example:</p><p>

```
git clone https://github.com/TessavdL/webserv webserv
```
  </p>
      <p>This will create a new directory called webserv and will clone the github repostiory into it. 
      Please note if you do not specify name_of_target_directory, the repostitory will be cloned into your working directory.</p>
    </li>
    <li>Press enter and you've successfully cloned the repostitory!</li>
  </ol>
</div>
<div class="run">
  <h2>How to run codyshell</h2>
  <ol>
    <li>Change your directory to the directory you've just cloned.</li><p>

```
cd <target_location>
```
  </p>
    <li>Use the makefile to build the project. Type in <code>make</code> into the terminal and press enter. The makefile will build the project and create an executable called codyshell.</li>
    <li>The makefile also allows you to rebuild the project with: <code>make re</code>, remove all object files with <code>make clean</code> and remove all object files and the executable with <code>make fclean</code>.</li>
    <li>Now you can run webserv by typing in <code>./webserv</code>. This will start the webserver with a default configuration</li>
    <li>If you wish to change the configuration of webserv you can start the program by giving it an argument with the path to your configuration file of choice.</li>
      <p>For example:</p><p>

```
./webserv myconf
```
  </p>
    <li>If you wish to exit codyshell you can use the <code>exit</code> command, press <code>ctrl + c</code> or close the terminal.</li>
  </ol>
</div>
</html>
