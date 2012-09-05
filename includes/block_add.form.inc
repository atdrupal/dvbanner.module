<?php
// $Id: block_add.form.inc 239 2008-10-09 09:25:37Z The Hong $

/**
 * Banner block adding form
 * 
 */
function dvbanner_block_add () {
  $form['type'] = array (
    '#type' => 'radios',
    '#title' => t('Block type'),
    '#required' => true,
    '#options' => array (
      'image' => t('Image'),
      'flv' => 'Flash video (*.flv)'
    ),
    '#default_value' => 'image',
    '#description' => 'Xin chọn kiểu block muốn tạo.'
  );
  
  $form['title'] = array (
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#required' => true,
    '#description' => 'Mỗi banner block cần có một tiêu đề thật gợi nhớ để có thể dễ dàng quản lý.'
    . '<strong>Chú ý:</strong> Tiêu đề của mỗi banner block là duy nhất.'
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  
  menu_set_active_item ('admin/build/block/dvbanner');

  return $form;
}

/**
 * Validator for banner block adding form
 * 
 */
function dvbanner_block_add_validate ($form_id, $form_values) {
  // Tên banner block không nên đặt trùng
  $number = db_result (db_query ("SELECT COUNT(*) FROM {dvbanner_block} WHERE title = '%s'", $form_values['title']));
   
  if ($number > 0) {
    form_set_error ('title', t('Banner block title has already existed.'));
  }
  
  // Type phải ở trong tập xác định
  if (!in_array ($form_values['type'], array ('image', 'flv'))) {
    form_set_error ('type', 'Giá trị cho trường dữ liệu <strong>block type</strong> là không hợp lệ.');
  }
}

/**
 * Submit handler for banner block adding form
 * 
 */
function dvbanner_block_add_submit ($form_id, $form_values) {
  $delta = db_next_id ('{dvbanner_block}_delta');
  
  db_query (
    "INSERT INTO {dvbanner_block} (delta, type, title, data) VALUES (%d, '%s', '%s', '%s')", 
    $delta, $form_values['type'], $form_values['title'], serialize (array ('limitation' => 1))
  );
  
  // Thông báo: đã tạo bannner block thành công
  drupal_set_message (t(
    "Created banner block <strong>!title</strong>", 
    array ('!title' => check_plain ($form_values['title']))
  ));
  
  // redirect to block setting form
  drupal_goto ('admin/build/block/configure/dvbanner/' . $delta, 'destination=admin/build/block/dvbanner');
}
