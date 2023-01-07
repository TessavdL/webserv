<?php
	$buffer = fopen('php://input', "r");
	$myfile = fopen("index.html", "w");
	file_put_contents("index.html", $buffer);
?>
