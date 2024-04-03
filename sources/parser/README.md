## ConfigParser

### Description

This library is used to parse a config file (in the JSON format) with a few caveats.

### Rules

In the configuration file, you should be able to:
- Choose the port and host of each ’server’.
- Setup the server_names or not.
- The first server for a host:port will be the default for this host:port (that means it will answer to all the requests that don’t belong to an other server).
- Setup default error pages.
- Limit client body size.
- Setup routes with one or multiple of the following rules/configuration (routes wont be using regexp):
    - Define a list of accepted HTTP methods for the route.
    - Define a HTTP redirection.
    - Define a directory or a file from where the file should be searched (for example, if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is /tmp/www/pouic/toto/pouet).
    - Turn on or off directory listing.
    - Set a default file to answer if the request is a directory.
    - Execute CGI based on certain file extension (for example .php).
    - Make it work with POST and GET methods.
    - Make the route able to accept uploaded files and configure where they should be saved.
        - Do you wonder what a CGI is?
        - Because you won’t call the CGI directly, use the full path as PATH_INFO.
        - Just remember that, for chunked request, your server needs to unchunk it, the CGI will expect EOF as end of the body.
        - Same things for the output of the CGI. If no content_length is returned from the CGI, EOF will mark the end of the returned data.
        - Your program should call the CGI with the file requested as first argument.
        - The CGI should be run in the correct directory for relative path file access.
        - Your server should work with one CGI (php-CGI, Python, and so forth).

You must provide some configuration files and default basic files to test and demonstrate every feature works during evaluation.


### Accepted keywords
- server (can be in http only)
- server_name (can be in server only)
- listen (can be in server only)
- location (can be in server only)
- fastcgi_pass (can be in location only)
- error_page (can be in http, server, location blocks)
- client_max_body_size (can be in http, server, location blocks)
- root (can be in http, server, location blocks)
- index (can be in http, server, location blocks)
- autoindex (can be in http, server, location blocks)

### Location
- Location is a context block like so:

```c++
location [modifier] [URI] {
    [directive] [value];
}
```

- The URI is the path to match.
- The modifier is one of the following:
    - =: exact match
    - ~: case-sensitive regular expression match
    - ~*: case-insensitive regular expression match
    - ^~: non-regular expression match


### References
- [Nginx Configuration](https://nginx.org/en/docs/beginners_guide.html)
- [Nginx Configuration](https://nginx.org/en/docs/http/ngx_http_core_module.html)