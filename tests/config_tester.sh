#!/bin/bash

WEBSERV="../webserv"

echo "Testing autoindex value\nExpected result: \nInvalid autoindex value\nResult:"
$WEBSERV ../configs/bad_configs/invalid_autoindex.conf

echo "\nTesting autoindex value\nExpected result: \nautoindex < empty directive\nResult:"
$WEBSERV ../configs/bad_configs/invalid_autoindex_empty.conf

echo "\nTesting autoindex value\nExpected result: \nautoindex on off < expected 1 argument\nResult:"
$WEBSERV ../configs/bad_configs/invalid_autoindex_multiple_arguments.conf

echo "\nTesting brackets with 1 bracket too little\nExpected result: \nConfig file is not valid\nResult:"
$WEBSERV ../configs/bad_configs/invalid_brackets.conf

echo "\nTesting brackets with 1 bracket too much\nExpected result: \nConfig file is not valid\nResult:"
$WEBSERV ../configs/bad_configs/invalid_brackets_2.conf

echo "\nTesting client_max_body_size\nExpected result: \nClient max body size too large, please add a value between 0 and 999\nResult:"
$WEBSERV ../configs/bad_configs/invalid_client_max_body_size.conf

echo "\nTesting error page values with invalid amount of error pages\nExpected result: \nInvalid character in error page's error code\nResult:"
$WEBSERV ../configs/bad_configs/invalid_error_page.conf

echo "\nTesting empty limit except directive\nExpected result: \nEmpty limit except directive\nResult:"
$WEBSERV ../configs/bad_configs/invalid_empty_limit_except.conf

echo "\nTesting limit except with invalid methods\nExpected result: \nInvalid limit_except argument\nResult:"
$WEBSERV ../configs/bad_configs/invalid_limit_except.conf

echo "\nTesting listen with invalid host and port\nExpected result: \nInvalid character found in listen port\nResult:"
$WEBSERV ../configs/bad_configs/invalid_listen.conf

echo "\nTesting empty location path\nExpected result: \nEmpty location path\nResult:"
$WEBSERV ../configs/bad_configs/location_path_empty.conf

echo "\nTesting location path and modifier too many arguments\nExpected result: \nToo many arguments for location path and modifier\nResult:"
$WEBSERV ../configs/bad_configs/location_too_many_arguments.conf

echo "\nTesting location block wrong modifier\nExpected result: \nInvalid optional modifier for location\nResult:"
$WEBSERV ../configs/bad_configs/location_wrong_optional_modifier.conf