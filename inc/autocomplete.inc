<?php
// $Id: autocomplete.inc 207 2008-10-09 05:05:32Z The Hong $

$type = arg (2);
$string = arg (3);
	
if ($type == 'profile') {
  $result = db_query (
    "SELECT pid, name FROM {dvbanner_profile} WHERE LOWER(name) LIKE LOWER('%%%s%%')", 
    $string
  );
  
  while ($v = db_fetch_object($result)) {
  	$matches[$v->name] = $v->name;
  }
  
  print drupal_to_js ($matches);
}
elseif (in_array ($type, array ('image', 'flv'))) {
	// The user enters a comma-separated list of tags. We only autocomplete the last tag.
	// This regexp allows the following types of user input:
	// this, "somecmpany, llc", "and ""this"" w,o.rks", foo bar
	$regexp = '%(?:^|,\ *)("(?>[^"]*)(?>""[^"]* )*"|(?: [^",]*))%x';
	preg_match_all($regexp, $string, $matches);
	$array = $matches[1];
	
	// Fetch last tag
	$last_string = trim(array_pop($array));
	if ($last_string != '') {
	  $result = db_query (
	    "SELECT bid, title FROM {dvbanner_item} WHERE LOWER(title) LIKE LOWER('%%%s%%') AND type = '%s'", 
	    $last_string, $type
	  );
	  
	  $prefix = count($array) ? implode(', ', $array) .', ' : '';
	  $matches = array();
	  
	  while ($v = db_fetch_object($result)) {
	    $n = $v->title;
	    // Commas and quotes in terms are special cases, so encode 'em.
	    if (strpos($v->title, ',') !== FALSE || strpos($v->title, '"') !== FALSE) {
	      $n = '"'. str_replace('"', '""', $v->title) .'"';
	    }
	    
	    $matches[$prefix . $n] = check_plain($v->title);
	  }
	  
	  print drupal_to_js ($matches);
	}
}
