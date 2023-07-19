<!DOCTYPE html>
<html>
<div class="welcome">
  <h1>Hi there, welcome to webserv 👋</h1>
  <subtitle>Webserv is an HTTP webserver that serves website content, handles form data and handles image uploading.
  Form data and image uploading is enabled through a Common Gateway Interface that executes php.
  Webserv responds to HTTP requests with the following HTTP methods: <code>GET</code>, <code>POST</code> and <code>DELETE</code>.
  Webserv is written according to the HTTP/1.1 protocol in the C++ programming language and created by vdkJelle and TessavdL.</subtitle>
</div>

<div class="webserver">
  <h2>What is a webserver?</h2>
    
  ![image](https://github.com/TessavdL/webserv/assets/72658914/f4986b2c-cfdb-4a3c-bd4d-6b75e9274467)

  <p>An HTTP web server is a software application that enables hosting and delivery of web content over the internet.
It handles requests from web browsers or other clients and responds by sending back the requested information or an error repsonse.
HTTP stands for Hypertext Transfer Protocol, which defines how information is requested and transmitted between server and client.</p>
<p>The HTTP request/response process goes as follows:</p>           
<ol>
  <li>A client (e.g., a web browser) sends an HTTP request to the web.</li>
  <li>An  HTTP web server receives the HTTP request.</li>
  <li>The server processes the HTTP request.</li>
  <li>The server returns an HTTP response to the client.</li>
  <li>The client receives the HTTP response.</li>
</ol>
</div>

<div class="request">
  <h2>What is an HTTP request?</h2>
  <p>An HTTP request is send by a client to the HTTP server. A valid HTTP request contains the following structure</p>
  <ol>
    <li>The first line is always the request line, consisting of an HTTP method (in our case <code>GET</code>, <code>POST</code>, <code>DELETE</code>), a Request-URI and the HTTP protocol version.</li>
    <li>The following lines consist of HTTP request headers. HTTP headers lets the client pass additional information with an HTTP request. An HTTP header consists of its case-insensitive name followed by a colon <code>:</code>, then by its value.</li>
    <li>An optional message body. In case of an HTTP request with the POST method the message body can contain data such as form data or image data.
  </ol>
  <p>An example of an HTTP request</p>
<p>

```
GET index.html HTTP/1.1
Host: www.example.com
Accept: */*
```
  </p>
  <p>
    When sending an HTTP request one should use the proper syntax. Every line ends with a CRLF. CR stands for Carriage return (ASCII 13, \r) and LF for Line Feed (ASCII 10, \n). Between the HTTP headers and the message body there is an extra CRLF.
  </p>
</div>

<div class="response">
  <h2>What is an HTTP response?</h2>
  <p>An HTTP response is send by the server to the client after the server has processed the HTTP request. A valid HTTP response contains the following structure</p>
  <ol>
    <li>The first line is always the status line, consisting of the HTTP protocol, a status code and a reason phrase. The status code is a code that gives the client information about the status of their request. Responses are grouped in classes: Informational (100-199), Succcessful (200-299), Redirection (300-399), Client error (400-499) and Server Errror (500-599). Webserv doesn't handle all status codes, but will send back the right status code class. The reason phrase gives a short explanation of the status code.</li>
    <li>The following lines consist of HTTP response headers. Like request HTTP headers, it is used to provide additional information to the responder. An HTTP response header also consists of its case-insensitive name followed by a colon <code>:</code>, then by its value.</li>
    <li>An optional message body. This can for example be an HTML document, image data, css file or the results of the program executed through the Common Gateway Interface. 
  </ol>
  <p>An example of an HTTP request</p>
<p>

```
HTTP/1.1 200 OK
Date: Wed, 21 Oct 2015 07:28:00 GMT
Server: Webserv1.0
Content-Type: text/html
Content-Length: 70

<!DOCTYPE html>
<html>
  <body>
    Hello, World!
  </body>
</html>
```
  </p>
  <p>
    An HTTP response has a similar syntax to an HTTP request. Every line ends with a CRLF. Between the HTTP response headers and the message body there is an extra CRLF.
  </p>
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
  <h2>How to run webserv</h2>
  <ol>
    <li>Change your directory to the directory you've just cloned.</li><p>

```
cd <target_location>
```
  </p>
    <li>Use the makefile to build the project. Type in <code>make</code> into the terminal and press enter. The makefile will build the project and create an executable called codyshell.</li>
    <li>The makefile also allows you to rebuild the project with: <code>make re</code>, remove all object files with <code>make clean</code> and remove all object files and the executable with <code>make fclean</code>.</li>
    <li>Now you can run webserv by typing in <code>./webserv</code>. This will start the webserver with a default configuration</li>
    <li>If you wish to change the configuration of webserv you can start the program by giving it an argument with the path to your configuration file of choice. For example:</li><p>

```
./webserv file.conf
```
  </p>
    <li>If you wish to exit webserv you can press <code>ctrl + c</code> or close the terminal.</li>
  </ol>
</div>

<div class="use">
  <h2>How to use webserv</h2>
  Webserv can be reached by visiting the following URL in the browser: <code>http://localhost/"path_here"</code>. If no portnumber is specified, webserv will listen to port <code>80</code>.
You can also specify a port number, which will work if the configuration file is set to handle that portnumber. For example: <code>http://localhost/"path_here":8080</code>.
If webserv is run with the standard configuration file you can go to <code>http://localhost</code> and you'll find an overview of webserv features: 
  <ul>
    <li>Standard HTML pages with images, CSS and JS.</li>
    <li>HTML page where you can submit a form with both GET and POST methods.</li>
    <li>HTML page where you can upload an image.</li>
    <li>HTML page that displays information regarding the site's PHP configuration, including: The current version of PHP your site is running, server information and environment.</li>
  </ul>
  Webserv can also be reached using <code>cURL</code> and has been tested with siege (https://formulae.brew.sh/formula/siege), where it reached 100% uptime with simple HTML requests and HTML request requesting a 8MB image with 200 clients at a time.

</div>

</html>
