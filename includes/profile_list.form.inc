<?php
// $Id: profile_list.form.inc 226 2008-10-09 08:09:39Z The Hong $

/**
 * Profiles listing form
 *
 */
function dvbanner_profile_list () {
  // Truy vấn tìm các profile hiện có
  $limit = 30;
  
  $q = pager_query ("SELECT pid, name FROM {dvbanner_profile} ORDER BY pid", $limit);
    
  $rows = array ();
    
  while ($v = db_fetch_object ($q)) {
    $rows[] = array ($v->pid, $v->name, implode (' | ', array (
      l(t('Edit'), "admin/build/block/dvbanner/profiles/$v->pid/edit"),
      $v->name == 'Default' ? '' : l(t('Delete'), "admin/build/block/dvbanner/profiles/$v->pid/delete")
    )));
  }
  
  $form[] = array (
    '#type' => 'markup',
    '#value' => theme ('table', array (t('ID'), t('Profile name'), t('Operations')), $rows) . theme ('pager', $limit)
  );
  
  return $form;
}
