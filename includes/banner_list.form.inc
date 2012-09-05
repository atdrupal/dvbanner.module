<?php
// $Id: banner_list.form.inc 238 2008-10-09 09:00:22Z The Hong $

/**
 * ...
 * 
 */
function dvbanner_banner_list () {
  // Delete the banner block
  if (is_numeric ($_GET['delete'])) {
    if (user_access('administer banner')) {
      if (dvbanner_block_delete ($_GET['delete'])) {
        drupal_set_message (t('Deleted banner block @delta', array ('@delta' => $_GET['delete'])));
      }
      else {
        drupal_set_message ('Not found block #'.$_GET['delete']);
      }
    }
    else {
      drupal_set_message ('You do not have permission to delete banner block.', 'error');
    }
  }

  $limit = 30;
  
  $table_header = array (
    array ('data' => 'ID', 'field' => 'delta', 'sort' => 'DESC'),
    array ('data' => t('Type'), 'field' => 'type'),
    array ('data' => t('Title'), 'field' => 'title'),
    '', ''
  );
  
  $q = pager_query (
    "SELECT * FROM {dvbanner_block}" . tablesort_sql ($table_header), 
    $limit
  );
  
  while ($block = db_fetch_object($q)) {
    $blocks[] = array (
      $block->delta,
      $block->type, 
      $block->title,
      l(theme('image', drupal_get_path ('module', 'dvbanner') . '/images/edit.png') . t('edit'), 'admin/build/block/configure/dvbanner/'. $block->delta, null, 'destination=' . $_GET['q'], null, false, true),
      l(theme('image', drupal_get_path ('module', 'dvbanner') . '/images/delete.png') . t('delete'), $_GET['q'], array ('class' => 'block-delete'), 'delete=' . $block->delta, null, false, true)
    );
  }
  
  $form[] = array (
    '#type' => 'markup',
    '#value' => theme ('table', $table_header, $blocks)
    . theme ('pager', null, $limit)
  );
  
  drupal_add_js ("$(function(){
    $('.block-delete').click(function(){
      return confirm ('Are you sure to delete this banner block?');
    });
  });", 'inline');
  
  $form['add'] = array (
    '#type' => 'submit',
    '#value' => t('Add new banner block')
  );

  return $form;
}

/**
 * Enter description here...
 *
 */
function dvbanner_banner_list_submit () {
  drupal_goto ('admin/build/block/dvbanner/add-block');
}
