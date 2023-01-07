#!/bin/bash

WEBSERV="../webserv"

echo "------------------------------_TESTING DUPLICATE DIRECTIVES_-----------------------------------"
echo "\nTesting duplicate directive in server block with double listen\nExpected result: \nDuplicate directive in server block\nResult:"
$WEBSERV ../configs/bad_configs/duplicate_directive_server_block.conf

echo "\nTesting duplicate directive in location block with double limit except\nExpected result: \nDuplicate directive in location block\nResult:"
$WEBSERV ../configs/bad_configs/duplicate_directive_location_block.conf

echo "------------------------------_TESTING AUTOINDEX_-----------------------------------"
echo "\nTesting autoindex with a wrong value value\nExpected result: \nInvalid autoindex value\nResult:"
$WEBSERV ../configs/bad_configs/autoindex_wrong_value.conf

echo "\nTesting autoindex with an empty value\nExpected result: \nautoindex < empty directive\nResult:"
$WEBSERV ../configs/bad_configs/autoindex_empty.conf

echo "\nTesting autoindex with multiple values\nExpected result: \nautoindex on off < expected 1 argument\nResult:"
$WEBSERV ../configs/bad_configs/autoindex_multiple_arguments.conf

echo "------------------------------_TESTING CLIENT MAX BODY SIZE_-----------------------------------"
echo "\nTesting client_max_body_size with a too large value\nExpected result: \nClient max body size too large, please add a value between 0 and 999\nResult:"
$WEBSERV ../configs/bad_configs/client_max_body_size_too_large.conf

echo "\nTesting client_max_body_size with an empty directive\nExpected result: \nclient_max_body_size < empty directive\nResult:"
$WEBSERV ../configs/bad_configs/client_max_body_size_empty.conf

echo "\nTesting client_max_body_size with no numbers\nExpected result: \nNo number in client max body size\nResult:"
$WEBSERV ../configs/bad_configs/client_max_body_size_no_numbers.conf

echo "------------------------------_TESTING BRACKET DETECTION_-----------------------------------"
echo "\nTesting brackets with 1 bracket too little\nExpected result: \nBrackets do not open and close properly\nResult:"
$WEBSERV ../configs/bad_configs/brackets_too_little.conf

echo "\nTesting brackets with 1 bracket too much\nExpected result: \nBrackets do not open and close properly\nResult:"
$WEBSERV ../configs/bad_configs/brackets_too_many.conf

echo "------------------------------_TESTING ERROR PAGES_-----------------------------------"
echo "\nTesting error page values with invalid amount of urls\nExpected result: \nInvalid character in error page's error code\nResult:"
$WEBSERV ../configs/bad_configs/error_page_double_url.conf

echo "\nTesting error page with an empty directive\nExpected result: \nError page has too few arguments\nResult:"
$WEBSERV ../configs/bad_configs/error_page_empty.conf

echo "\nTesting error page with missing status codes\nExpected result: \nError page has too few arguments\nResult:"
$WEBSERV ../configs/bad_configs/error_page_missing_status_code.conf

echo "------------------------------_TESTING LIMIT EXCEPT_-----------------------------------"
echo "\nTesting empty limit except directive\nExpected result: \nEmpty limit except directive\nResult:"
$WEBSERV ../configs/bad_configs/limit_except_empty.conf

echo "\nTesting limit except with invalid methods\nExpected result: \nInvalid limit_except argument\nResult:"
$WEBSERV ../configs/bad_configs/limit_except_invalid_method.conf

echo "\nTesting limit except in server block\nExpected result: \nInvalid directive in server block\nResult:"
$WEBSERV ../configs/bad_configs/limit_except_in_server_block.conf

echo "------------------------------_TESTING LISTEN_-----------------------------------"
echo "\nTesting listen with an empty directive\nExpected result: \nListen directive empty\nResult:"
$WEBSERV ../configs/bad_configs/listen_empty.conf

echo "\nTesting listen with invalid host and port\nExpected result: \nInvalid character found in listen port\nResult:"
$WEBSERV ../configs/bad_configs/listen_invalid_port.conf

echo "\nTesting listen in a location block\nExpected result: \nInvalid directive in location block\nResult:"
$WEBSERV ../configs/bad_configs/listen_in_location.conf

echo "------------------------------_TESTING LOCATION BLOCK SYNTAX_-----------------------------------"
echo "\nTesting empty location path\nExpected result: \nEmpty location path\nResult:"
$WEBSERV ../configs/bad_configs/location_path_empty.conf

echo "\nTesting location path and modifier too many arguments\nExpected result: \nToo many arguments for location path and modifier\nResult:"
$WEBSERV ../configs/bad_configs/location_too_many_arguments.conf

echo "\nTesting location block wrong modifier\nExpected result: \nInvalid optional modifier for location\nResult:"
$WEBSERV ../configs/bad_configs/location_wrong_optional_modifier.conf

echo "\nTesting location block wrong location directive name\nExpected result: \nInvalid location block start\nResult:"
$WEBSERV ../configs/bad_configs/location_block_invalid_name.conf

echo "------------------------------_TESTING INDEX_-----------------------------------"
echo "\nTesting index with an empty directive\nExpected result: \nEmpty index directive\nResult:"
$WEBSERV ../configs/bad_configs/index_empty.conf

echo "------------------------------_TESTING CGI_-----------------------------------"
echo "\nTesting cgi with an empty directive\nExpected result: \nCGI has an incorrect amount of arguments\nResult:"
$WEBSERV ../configs/bad_configs/cgi_empty.conf

echo "\nTesting cgi in the server block\nExpected result: \nInvalid directive in server block\nResult:"
$WEBSERV ../configs/bad_configs/cgi_in_server_block.conf

echo "\nTesting cgi with too many arguments\nExpected result: \nCGI has an incorrect amount of arguments\nResult:"
$WEBSERV ../configs/bad_configs/cgi_too_many_arguments.conf

echo "------------------------------_TESTING ROOT_-----------------------------------"
echo "\nTesting root with an empty directive\nExpected result: \nroot < empty directive\nResult:"
$WEBSERV ../configs/bad_configs/root_empty.conf

echo "\nTesting root with multiple arguments\nExpected result: \nroot /var/www/html /var/www/html/cgi-bin < expected 1 argument\nResult:"
$WEBSERV ../configs/bad_configs/root_multiple_values.conf

echo "------------------------------_TESTING SERVER NAME_-----------------------------------"
echo "\nTesting server name with an empty directive\nExpected result: \nServer name directive empty\nResult:"
$WEBSERV ../configs/bad_configs/server_name_empty.conf

echo "\nTesting server name in a location block\nExpected result: \nInvalid directive in location block\nResult:"
$WEBSERV ../configs/bad_configs/server_name_in_location.conf

echo "------------------------------_TESTING RETURN_-----------------------------------"
echo "\nTesting return with an empty directive\nExpected result: \nReturn has an incorrect amount of arguments\nResult:"
$WEBSERV ../configs/bad_configs/return_empty.conf

echo "\nTesting return with a character in the status code\nExpected result: \nInvalid character in return status code\nResult:"
$WEBSERV ../configs/bad_configs/return_invalid_status_code.conf

echo "\nTesting return with only a path\nExpected result: \nInvalid character in return status code\nResult:"
$WEBSERV ../configs/bad_configs/return_only_path.conf

echo "------------------------------_TESTING REWRITE_-----------------------------------"
echo "\nTesting rewrite with an empty directive\nExpected result: \nrewrite < empty directive\nResult:"
$WEBSERV ../configs/bad_configs/rewrite_empty.conf

echo "\nTesting rewrite with more than 1 value\nExpected result: \nrewrite /var/www/html/ https://www.youtube.com/c/BrodieThatDood/shorts < expected 1 argument\nResult:"
$WEBSERV ../configs/bad_configs/rewrite_too_many_arguments.conf