import http.server

server_address = ('',8000)
server_class = http.server.HTTPServer
handler_class = http.server.CGIHTTPRequestHandler
httpd = server_class(server_address, handler_class)
httpd.serve_forever()