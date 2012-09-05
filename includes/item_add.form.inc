<?php
// $Id: item_add.form.inc 240 2008-10-09 09:29:03Z The Hong $

/**
 * Banner item adding form
 * 
 */
function dvbanner_item_add () {
  $form = dvbanner_item_attributes ();
  
  $form['submit'] = array (
    '#type' => 'submit',
    '#value' => t('Add new banner item')
  );

  // ...
  menu_set_active_item ('admin/build/block/dvbanner/banner');
  
  return $form;
}

/**
 * Validator for banner item adding form
 * 
 */
function dvbanner_item_add_validate ($form_id, $form_values) {
  // Chắc chắn là tên không bị trùng, vì sử dụng tính năng autocomplete ở biểu mẫu cấu hình block
  $number = db_result (db_query (
    "SELECT COUNT(*) FROM {dvbanner_item} WHERE title = '%s'",
    $form_values['title']
  ));
  
  if ($number > 0) {
    form_set_error ('title', t('Item name is already existed. Please choose another one.'));
  }
  else {
    // Lấy thông tin tập tin tải lên
    $file = file_check_upload('file');
    
    // Người dùng không upload file
    if (!$file) {
      // Người dùng phải nhập vào hoặc path, hoặc upload file
      if (empty($form_values['path'])) {
        form_set_error ('path', t('You have to enter path or file field.'));
      }
      // Đảm bảo URL hợp lệ
      elseif (preg_match ('|^http://|', $form_values['path']) && !valid_url ($form_values['path'], true)) {
        form_set_error ('path', t('Path is an invalid path URL.'));
      }
    }
    else {
      // Tập tin tải nên chỉ thuộc một số định dạng nhất định
      if (!preg_match('/\.(bmp|gif|jpeg|jpg|png|flv|wmv|avi|swf)$/i', $file->filename)) {
        form_set_error ('file', t('Only accept image, flash, video file type for file field.'));
      } 
    }
  }
  
  // Link nếu nhập vào phải là valid URL 
  if (!empty ($form_values['link']) && !valid_url ($form_values['link'])) {
    form_set_error ('link', t('Path is an invalid link URL.'));
  }
}

/**
 * Submit handler for banner item adding form
 * 
 */
function dvbanner_item_add_submit ($form_id, $form_values) {
  $file = file_check_upload ('file');
  
  if ($file) {
    $file = file_save_upload ($file, 'dvbanner/' . $file->filename);
    
    if (!$file) {
      drupal_set_message (t('Failed to upload file.'), 'error');
      
      return;
    }
    
    $form_values['path'] = $file->filepath;
  }
  
  // Lưu thông tin của banner item vào CSDL
  $data = array ();
  $data['path'] = $form_values['path'];
  if (!preg_match('|flv$|i', $form_values['path'])) {
    $data['link'] = $form_values['link'];
  }
  
  db_query (
    "INSERT INTO {dvbanner_item} (type, title, status, click, data, weight) VALUES ('%s', '%s', %d, 0, '%s', %d)",
    preg_match ('|flv$|i', 
    $form_values['path']) ? 'flv' : 'image', 
    $form_values['title'], 
    $form_values['status'], 
    serialize ($data), 
    $form_values['weight']
  );
  
  // Thông báo đến người dùng, banner item đã tạo thành công
  drupal_set_message (t("Created banner <strong>!title</strong>.", array (
    '!title' => check_plain ($form_values['title'])
  )));
  
  // Chuyển hướng người dùng đến trang liệt kê các banner block
  drupal_goto ('admin/build/block/dvbanner/banner');
}
