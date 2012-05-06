<!DOCTYPE html>
<!--
File for experimenting with client side manipulation of SVG.
-->
<html>
<head>
<?php
$af = $_REQUEST["af"];
?>
<title>graphgame</title>
<style type="text/css">
body {
    font-family: sans-serif;
}
a:link {
    text-decoration: none;
    color: black;
}
a:visited {
    text-decoration: none;
    color: black;
}
a:hover {
    text-decoration: none;
    color: red;
}
a:active {
    text-decoration: none;
    color: red;
}
#colors {
    width: 400px;
}
#numTries {
    width: 50px;
}
</style>
<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"></script>
<script type="text/javascript" src="js/GraphGame.js"></script>
</head>
<body>
<!-- width="1110" height="704" -->
<?php if ($af == "us-states") { ?>
<embed id="svg" src="../svg/Map_of_USA_without_state_names.svg" />
<?php } else if ($af == "us-counties") { ?>
<embed id="svg" src="../svg/USA_Counties_with_FIPS_and_names.svg" />
<?php } ?>
<br/>
<span id="info"></span>
<br/>
Selected Color: <input type="text" id="fill" value="salmon"/>
 Search: <input type="text" id="search" value=""/><a href="#" id="clearSearch">X</a>
 <input type="button" id="random" value="Select Random"/>
 <input type="button" id="reset" value="Reset"/>
<br/>
<b>Breadth First Search</b> <input type="button" id="bfs-demo" value="Run"/>
<br/>
<b>Greedy Coloring</b> 
    Colors: <input type="text" id="colors" value="lightseagreen,lightblue,lightsalmon,beige,plum,lightcoral" />
    Retry: <input type="text" id="numTries" value="10" />
<input type="button" id="greedy-coloring" value="Run"/>
<br/>
<div id="searchResults"></div>
<?php 
$defaultNodeFill = "white";
if ($af == "us-states") {
    $defaultNodeFill = "white";
} else if ($af == "us-counties") {
    $defaultNodeFill = "#d0d0d0";
} else {
    die("Unrecognized preset");
}
echo "<input type=\"hidden\" id=\"af\" value=\"$af\" />\n";
echo "<input type=\"hidden\" id=\"defaultNodeFill\" value=\"$defaultNodeFill\" />\n";
?>
</body>
</html>
