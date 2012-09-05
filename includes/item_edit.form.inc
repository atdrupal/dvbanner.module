<?php
// $Id: item_edit.form.inc 222 2008-10-09 07:59:04Z The Hong $

/**
 * Banner item editing form
 *
 */
function dvbanner_item_edit () {
  $form = dvbanner_item_attributes ();

  // Tải lên các thông tin cần thiết và thiết lập làm giá trị mặc định 
  if ($bid = arg (6)) {
    $form['bid'] = array (
     '#type' => 'value',
     '#value' => $bid
    );
  
    $banner = db_fetch_object (db_query (
      "SELECT * FROM {dvbanner_item} WHERE bid = %d", $bid
    ));
    
    $banner->data = unserialize ($banner->data);
    
    foreach ($banner->data as $key => $val) {
      $banner->{$key} = $val;
    }
    
    unset ($banner->data);
    
    foreach ($banner as $key => $val) {
      switch ($key) {
        case 'title':
          $form['title']['#default_value'] = $val;
        case 'status':
          $form['displaying']['status']['#default_value'] = $val;
        case 'weight':
          $form['displaying']['weight']['#default_value'] = $val;
        case 'path':
          $form['resource']['path']['#default_value'] = $val;
        case 'link':
          $form['resource']['link']['#default_value'] = $val;
      }
    }
    
    if ($banner->type == 'flv') {
      // Không cần sử dụng field link
      unset ($form['resource']['link']);
      
      // Ảnh mặc định sẽ hiển thị ở player khi flash video chưa được chơi
      $form['resource']['image'] = array (
        '#type' => 'textfield',
        '#title' => 'Hình thumbnail',
        '#description' => 'Đường dẫn đến hình ảnh được hiển thị trước khi flash video được chơi. Nếu giá trị nhập vào là rỗng, hệ thống sử dụng ảnh thumbnail mặc định từ player profile tương ứng.',
        '#default_value' => $banner->image
      );
    }
  }
  
  $form['submit'] = array (
    '#type' => 'submit',
    '#value' => t('Update')
  );
  
  // Hiển thị system tabs
  menu_set_active_item('admin/build/block/dvbanner/banner');
  
  return $form;
}

/**
 * Validator for banner item editing form
 * 
 */
function dvbanner_item_edit_validate ($form_id, $form_values) {
  // Không có Banner item ID
  if (!is_numeric ($form_values['bid'])) {
    form_set_error ('', t('Invalid banner item value.'));
  }
  else {
    // Chắc chắn là tên không bị trùng, vì sử dụng tính năng autocomplete ở biểu mẫu cấu hình block
    $number = db_result (db_query (
      "SELECT COUNT(*) FROM {dvbanner_item} WHERE title = '%s' AND bid != %d",
      $form_values['title'], $form_values['bid']
    ));
    
    if ($number > 0) {
      form_set_error ('title', t('Item name is already existed. Please choose another one.'));
    }
    else {
      // Lấy thông tin tập tin tải lên
      $file = file_check_upload ('file');
      
      // Người dùng không upload file
      if (!$file) {
        // Người dùng phải nhập vào hoặc path, hoặc upload file
        if (empty($form_values['path'])) {
          form_set_error ('path', t('You have to enter path or file field.'));
        }
        // Đảm bảo URL hợp lệ
        elseif (preg_match ('|^http://|', $form_values['path']) && !valid_url ($form_values['path'], true)) {
          form_set_error ('path', t('You entered an invalid URL.'));
        }
      }
      else {
        // Tập tin tải nên chỉ thuộc một số định dạng nhất định
        if (!preg_match('/\.(bmp|gif|jpeg|jpg|png|flv|wmv|avi|swf)$/i', $file->filename)) {
          form_set_error ('file', t('Only accept image, flash, video file type for file field.'));
        }
      }
    }
  }
  
  // Link nếu nhập vào phải là valid URL 
  if (!empty ($form_values['link']) && !valid_url ($form_values['link'])) {
    form_set_error ('link', t('Path is an invalid link URL.'));
  }
}

/**
 * Submit handler for banner item editing form
 *
 */
function dvbanner_item_edit_submit ($form_id, $form_values) {
  $bid = $form_values['bid'];
  
  $data = array ();
  
  // Tải lên tập tin mới =? xoá tập tin cũ
  $file = file_check_upload ('file');
  if ($file) {
    // Tìm đường dẫn đến tập tin hiện tại
    $banner_data = db_result (db_query (
      "SELECT data FROM {dvbanner_item} WHERE bid = %d", 
      $bid
    ));
    
    $banner_data = unserialize ($banner_data);
      
    // Chắn chắn là đường dẫn nội
    if (!preg_match('|/|', $banner_data->path)) {
      file_delete ($banner_data->path);
    }
    
    $file = file_save_upload ($file, 'dvbanner/' . $file->filename);
    if (!$file) {
      drupal_set_message (t('Failed to upload file.'), 'error');
      
      return;
    }
    
    $data['path'] = $file->filepath;
  }
  else {
    $data['path'] = $form_values['path']; 
  }
  
  if ($form_values['link']) {
    $data['link'] = $form_values['link'];
  }
  
  if ($form_values['image']) {
    $data['image'] = $form_values['image'];
  }
  
  // Update data in DB
  db_query (
    "UPDATE {dvbanner_item} SET title = '%s', status = %d, data = '%s', weight = %d WHERE bid = %d",
    $form_values['title'], 
    $form_values['status'], 
    serialize ($data), 
    $form_values['weight'], 
    $bid
  );
  
  // Thông báo đến người dùng: Banner item đã được cập nhật thành công
  drupal_set_message (t(
    "Updated banner !title", array (
    '!title' => check_plain($form_values['title'])
  )));
  
  // Chuyển hướng người dùng đến trang liệt kê các banner block
  drupal_goto ('admin/build/block/dvbanner/banner');
}
