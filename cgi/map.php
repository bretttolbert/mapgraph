<?php header('Content-type: text/html; charset=utf-8'); ?>
<!DOCTYPE html>
<!--
File for experimenting with client side manipulation of SVG.
-->
<html>
<meta http-equiv="Content-type" content="text/html;charset=UTF-8">
<head>
<?php
$af = $_REQUEST["af"];
?>
<title>mapgraph</title>
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
.SmallNumericField {
    width: 30px;
    text-align: center;
}
label {
    font-size: 10px;
}
#loaderContainer {
    position: absolute;
    display: table;
    top: 0px;
    left: 0px;
    width: 100%;
    height: 100%;
}
#loaderPosition {
    display: table-cell;
    width: 100%;
    text-align: center;
    vertical-align: middle;
}
</style>
<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"></script>
<script type="text/javascript" src="js/mapgraph.js"></script>
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
<label for="fill">Selected Color: </label><input type="text" id="fill" value="salmon"/>
<label for="search">Search: </label><input type="text" id="search" value=""/><a href="#" id="clearSearch">X</a>
 <input type="button" id="random" value="Select Random"/>
 <input type="button" id="reset" value="Reset"/>
<br/>
<b>Breadth First Search</b> <input type="button" id="bfs-demo" value="Run"/>
<br/>
<b>Greedy Coloring</b> 
    <label for="colors">Colors: </label>
    <input type="text" id="colors" value="lightseagreen,lightblue,lightsalmon,beige,plum,lightcoral" />
    <label for="numTries">Retry: </label>
    <input class="SmallNumericField" type="text" id="numTries" value="10" />
<input type="button" id="greedy-coloring" value="Run"/>
<br/>
<div id="searchResults"></div>
<b>Quick Facts</b>: <select id="quickFactsSel"></select> 
    <label for="scaleSel">Scale: </label> 
    <select id="scaleSel">
        <option value="linear">Linear</option>
        <option value="logarithmic">Logarithmic</option>
        <option value="stdDeviation" selected="selected">Standard Deviation</option>
    </select>
    <span id="baseSpan">
        <label for="base">Base: </label>
        <input class="SmallNumericField" type="text" id="base" value="2" />
    </span>
    <span id="stdDeviationParamsSpan">
        <input class="SmallNumericField" type="text" id="minSigma" value="-1" />
        &sigma; <label>to</label>
        <input class="SmallNumericField" type="text" id="maxSigma" value="1" />
        &sigma;
    </span>
    <input type="button" value="Visualize" id="visualizeQuickFact" />
<br/>
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
<textarea id="log" rows="6" cols="80"></textarea>
<div id="loaderContainer">
    <div id="loaderPosition">
    <?php
        $size = 2;
        $width = 54*$size;
        $height = 55*$size;
        echo "<img src=\"img/ajax-loader.gif\" width=\"$width\" height=\"$height\"/>";
    ?>
    </div>
</div>
</body>
</html>
