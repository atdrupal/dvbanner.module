<?php
// $Id: report.form.inc 225 2008-10-09 08:08:48Z The Hong $

/**
 * Reporting form
 *
 */
function dvbanner_report () {
  $limit = 100;
  $sql = "SELECT c.cid, c.hostname, c.timestamp, i.title"
    . " FROM {dvbanner_click} c"
    . " INNER JOIN {dvbanner_item} i ON i.bid = c.bid";
  
  $header = array (
    array ('data' => 'ID', 'width' => '41px',   'field' => 'c.cid'),
    array ('data' => 'Banner', 'width' => '141px',  'field' => 'i.title'),
    array ('data' => 'Hostname', 'field' => 'c.hostname'),
    array ('data' => 'Time', 'field' => 'c.timestamp'),
  );
  
  $query = pager_query ($sql . tablesort_sql ($header), $limit);
  while ($v = db_fetch_object ($query)) {
    $rows[] = array ($v->cid, $v->title, $v->hostname, format_date ($v->timestamp));
  }
  
  $form[] = array (
    '#type' => 'markup',
    '#value' => theme ('table', $header, $rows) . theme ('pager', NULL, $limit)
  );
  
  return $form;
}
