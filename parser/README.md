## ConfigParser

### Description

This library is used to parse the config file (in the JSON format) with a few caveats.

### Rules

1. The config file must be in the JSON format.
2. The config file must contain the following fields:
	- `host` - the host to listen on
	- `port` - the port to listen on
	- `server_name` - the server name
	- `error_page` - the path to the error page
	- `client_max_body_size` - the maximum size of the body
	- `location` - the location block
3. The `location` block must contain the following fields:
	- `path` - the path to the location
	- `root` - the root directory
	- `index` - the index file
	- `autoindex` - the autoindex flag
	- `methods` - the list of allowed methods
	- `cgi` - the cgi block
4. The `cgi` block must contain the following fields:
	- `path` - the path to the cgi script
	- `extension` - the extension of the cgi script
	- `method` - the method to use to run the cgi script
	- `cgi_pass` - the cgi pass block
5. Floating point numbers are not supported.
6. The `autoindex` flag must be either `true` or `false`.
7. The `methods` list must contain at least one method.
8. The `method` field must be either `GET` or `POST`.
9. Invalid syntax will result in an exception being thrown.
10. Error pages will be set to the default error page if the path is invalid or if no error page is specified.
