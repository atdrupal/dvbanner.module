<?php
// $Id: dvbanner.admin.inc 523 2008-11-28 09:18:47Z The Hong $

/**
 * Delete a banner block
 */
function dvbanner_block_delete ($delta) {
  if (db_affected_rows (db_query ('DELETE FROM {dvbanner_block} WHERE delta = %d', $delta))) {
    db_query ("DELETE FROM {blocks} WHERE module = 'dvbanner' AND delta = %d", $delta);
    db_query ("DELETE FROM {dvbanner_block} WHERE delta = %d", $delta);
    db_query ("DELETE FROM {dvbanner_block_items} WHERE delta = %d", $delta);

    // delete xml cached file
    file_delete (file_directory_path () . "/dvbanner/playlist/{$delta}.xml");

    return true;
  }

  return false;
}

/**
 * Delete a banner item
 *
 * @param int $biid banner item ID
 */
function dvbanner_item_delete ($biid) {
  if ($result = db_result (db_query(
    "SELECT data FROM {dvbanner_item} WHERE bid = %d", $biid)
  )) {
    // Xoá thông tin ở bảng thống kê click
    db_query ("DELETE FROM {dvbanner_click} WHERE bid = %d", $biid);

    // Xoá tập tin
    $data = unserialize ($result);
    file_delete (check_url($data['path']));

    // Bỏ item ra khỏi các banner block đang chứa nó
    db_query ("DELETE FROM {dvbanner_block_items} WHERE bid = %d", $biid);

    // Xóa thông tin
    db_query ('DELETE FROM {dvbanner_item} WHERE bid = %d', $biid);

    return true;
  }

  return false;
}

/**
 * Helper to list the current banner blocks
 *
 */
function _dvbanner_block_list () {
  // we stored our block region at {dvbanner_block}
  $q = db_query (
    "SELECT delta, title FROM {dvbanner_block} ORDER BY delta"
  );

  while ($v = db_fetch_object ($q)) {
    $blocks[$v->delta] = array (
      'info' => '[BANNER] '.$v->title
    );
  }

  return $blocks;
}

/**
 * Helper for admin pages wrapper
 */
function _dvbanner_admin ($page) {
  switch ($page) {
    case 'dvbanner_banner_list':
      require_once dirname (__FILE__) . '/includes/banner_list.form.inc';
      return drupal_get_form ('dvbanner_banner_list');

    case 'dvbanner_item_list':
      require_once dirname (__FILE__) . '/includes/item_list.form.inc';

      return drupal_get_form ('dvbanner_item_list');

    case 'dvbanner_report':
      require_once dirname (__FILE__) . '/includes/report.form.inc';

      return drupal_get_form ('dvbanner_report');

    case 'dvbanner_profile_list':
      if (preg_match ('|^admin/build/block/dvbanner/profiles/[0-9]+/edit$|', $_GET['q'])) {
        require_once dirname (__FILE__) . '/includes/profile_edit.form.inc';

        return drupal_get_form ('dvbanner_profile_edit');
      }
      elseif (preg_match ('|^admin/build/block/dvbanner/profiles/[0-9]+/edit$|', $_GET['q'])) {
        require_once dirname (__FILE__) . '/includes/profile_delete.form.inc';

        return drupal_get_form ('dvbanner_profile_delete');
      }
      else {
        require_once dirname (__FILE__) . '/includes/profile_list.form.inc';

        return drupal_get_form ('dvbanner_profile_list');
      }

    case 'dvbanner_profile_add':
      require_once dirname (__FILE__) . '/includes/profile_add.form.inc';

      return drupal_get_form ('dvbanner_profile_add');

    case 'dvbanner_item_add':
      require_once dirname (__FILE__) . '/includes/item_add.form.inc';

      return drupal_get_form ('dvbanner_item_add');

    case 'dvbanner_item_edit':
      require_once dirname (__FILE__) . '/includes/item_edit.form.inc';

      return drupal_get_form ('dvbanner_item_edit');

    case 'dvbanner_block_add':
      require_once dirname (__FILE__) . '/includes/block_add.form.inc';

      return drupal_get_form ('dvbanner_block_add');

  }
}
