<?php
// $Id: item_list.form.inc 239 2008-10-09 09:25:37Z The Hong $

/**
 * Banner items listing
 * 
 */
function dvbanner_item_list () {
  // Xoá banner item
  if (is_numeric ($_GET['delete'])) {
    if (user_access('administer banner')) {
      if (dvbanner_item_delete ($_GET['delete'])) {
        // Thông báo cho người dùng
        drupal_set_message ('Deleted banner #'.$_GET['delete']);
      }
      else {
        drupal_set_message ('Banner #'.$_GET['delete'] . " not found.", "error");
      }
    }
    else {
      drupal_set_message ('You do not have permission to delete banner.', 'error');
    }
  }
  
  $limit = 30;
  
  $table_header = array (
    array ('data' => 'ID', 'field' => 'bid', 'sort' => 'DESC'),
    array ('data' => t('Type'), 'field' => 'type'),
    array ('data' => t('Title'), 'field' => 'title'),
    array ('data' => t('Status'), 'field' => 'status'),
    array ('data' => t('Click'), 'field' => 'click'),
    array ('data' => t('Weight'), 'field' => 'weight'),
    '', ''
  );
  
  $q = pager_query ("SELECT bid, type, title, status, click, weight FROM {dvbanner_item}" . tablesort_sql ($table_header), $limit);
  
  while ($banner = db_fetch_object($q)) {
    $banners[] = array(
      $banner->bid,
      $banner->type,
      $banner->title,
      $banner->status ? 'true' : 'false',
      $banner->click,
      $banner->weight,
      l(theme('image', drupal_get_path ('module', 'dvbanner') . '/images/edit.png') . t('edit'), 'admin/build/block/dvbanner/banner/edit/'.$banner->bid, null, null, null, false, true),
      l(theme('image', drupal_get_path ('module', 'dvbanner') . '/images/delete.png') . t('delete'), $_GET['q'], array ('class' => 'banner-item-delete'), 'delete='.$banner->bid, null, null, true)
    );
  }
    
  drupal_add_js ("$(function(){
    $('.banner-item-delete').click(function(){
     return confirm ('Are you sure to delete this banner item?');
    });
  });", 'inline');
  
  $form[] = array (
    '#type' => 'markup',
    '#value' => theme ('table', $table_header, $banners)
    . theme ('pager', null, $limit)
  );
  
  $form['add'] = array (
    '#type' => 'submit',
    '#value' => t('Add new banner item')
  );
  
  return $form;
}

/**
 * Submit handler
 */
function dvbanner_item_list_submit ($form_id, $form_values) {
  drupal_goto ('admin/build/block/dvbanner/add-item');
}
