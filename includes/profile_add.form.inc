<?php
// $Id: profile_add.form.inc 229 2008-10-09 08:26:18Z The Hong $

/**
 * Player profile adding form
 *
 */
function dvbanner_profile_add () {
  // Thiết lập lại breadcrumb cho dễ di chuyển
  $bc = drupal_get_breadcrumb();
  $bc[] = l(t('Banners'), 'admin/build/block/dvbanner');
  $bc[] = l(t('Profiles'), 'admin/build/block/dvbanner/profiles');
  drupal_set_breadcrumb($bc);
  
  // Thiết lập lại tiêu đề cho phù hợp
  drupal_set_title (t('Add new player profile'));
  
  $form = array (
    '#attributes' => array('enctype' => 'multipart/form-data')
  );
  
  // Profile name
  $form['name'] = array (
    '#type' => 'textfield',
    '#title' => t('Profile name'),
    '#description' => t('Unique name for your player profile.'),
    '#required' => true
  );
  
  // Get basic profile form elements
  $form += dvbanner_profile_attributes ();
  
  // Cần có submit button
  $form['buttons'] = array (
    'submit' => array (
      '#type' => 'submit',
      '#value' => t('Save')
    )
  );

  return $form;
}

/**
 * Validator for player profile adding form
 *
 */
function dvbanner_profile_add_validate ($form_id, $form_values) {
  // Đảm bảo tên profile không trùng lắp
  $number = db_result (db_query (
    "SELECT COUNT(*) FROM {dvbanner_profile} WHERE name = '%s'",
    $form_values['name']
  ));
  
  if ($number >= 1) {
    form_set_error ('name', t('Profile name has already existed. Please select another one.'));
  }
  
  // Basic settings
  //// height phải là giá trị số
  if (!preg_match ('|^[0-9]+$|', $form_values['height'])) {
    form_set_error ('height', t("Height must be a number."));
  }
  
  //// width phải là giá trị số
  if (!preg_match ('|^[0-9]+$|', $form_values['width'])) {
    form_set_error ('width', t("Width must be a number."));
  }
  
  //// Đảm bảo tập tin tải lên phải là một ảnh
  if ($image = file_check_upload ('image')) {
    $image_info = image_get_info($image->filepath);
    
    if (!$image_info || !$image_info['extension']) {
      form_set_error('image', t('The uploaded file was not an image.'));
    }
  }
  
  //// Các tông màu phải là hexa value
  foreach (array ('backcolor', 'frontcolor', 'lightcolor', 'screencolor') as $field) {
    if (!preg_match ('|^#[0-9ABCDEF]+$|i', $form_values[$field])) {
      form_set_error ($field, t('!field was not a valid value.', array ('!field' => $field)));
    }
  }
  
  // Display settings
  //// logo: Tập tin tải lên phải là một ảnh
  if ($logo = file_check_upload ('logo')) {
    $logo_info = image_get_info($logo->filepath);
    
    if (!$logo_info || !$logo_info['extension']) {
      form_set_error('logo', t('The uploaded file was not an image.'));
    }
  }
  
  // Biến trợ giúp việc kiểm tra các element gọn gàng hơn
  $_bool = array ('false', 'true');
  
  //// overstretch, show icon, show equalizer, repeat: đảm bảo giá trị trong tập giá trị định trước
  $values_set = array (
    'overstretch'    => array ('false', 'true', 'fit', 'none'),
    'showicons'      => $_bool,
    'showeq'         => $_bool,
    'shownavigation' => $_bool,
    'showstop'       => $_bool,
    'showdigits'     => $_bool,
    'usefullscreen'  => $_bool,
    'autostart'      => $_bool,
    'repeat'         => array ('false', 'list', 'true'),
    'shuffle'        => $_bool
  );
  
  foreach ($values_set as $key => $val) {
    if (!in_array ($form_values[$key], $val)) {
      form_set_error (
        $key, 
        t("Invalid valid value for @field field.", array ('@field' => $key)
      ));
    }
  }
  
  // Playlist
  if (!empty ($form_values['rotatetime']) && !is_numeric ($form_values['rotatetime'])) {
    form_set_error ('rotatetime' , t('Rotate time must be an integer.'));
  }
}

/**
 * Submit handler for player profile adding form
 * 
 */
function dvbanner_profile_add_submit ($form_id, $form_values) {
  // Tạo ID cho profile
  $pid = db_next_id ('{dvbanner_profile}_pid');
  
  // Nếu có tải tập tin default image/logo
  $image = file_check_upload ('image');
  $logo = file_check_upload ('logo');
  
  if ($image || $logo) {
    // Tạo thư mục chứa các tập tin của profile khi cần thiết
    $profile_path = file_directory_path () . '/dvbanner/profiles/' . $pid;
    file_check_directory ($profile_path, FILE_CREATE_DIRECTORY);
    
    // Nếu tải lên ảnh mặc định
    if ($image) {
      if ($image  = file_save_upload ($image, $profile_path . '/' . $image->filename)) {
        $form_values['image'] = $image->filepath;
      }
    }
    
    // Nếu tải lên logo
    if ($logo) {
      if ($logo   = file_save_upload ($logo, $profile_path . '/' . $logo->filename)) {
        $form_values['logo'] = $logo->filepath;
      }
    }
  }
  
  // Tạo biến $data để phù hợp cho việc lưu trữ  
  foreach ($form_values as $key => $val) {
    // Không cần lưu các giá trị rỗng hoặc 'default'
    if ($val != 'default') {
      // chỉ chấp nhận một số field xác định
      $allowed_fields = array (
        'height', 'width', 'image', 
        'backcolor', 'frontcolor', 'lightcolor', 'screencolor', 
        'logo', 'overstretch', 'showicons', 'showeq', 
        'shownavigation', 'showstop', 'showdigits', 'usefullscreen', 'showdownload',
        'autostart', 'repeat', 'shuffle', 'rotatetime'
      );
      
      if (in_array ($key, $allowed_fields)) {
        $data[$key] = $val;
      }
    }
  }
  
  // Truy vấn
  if (count ($data)) {
    db_query (
      "INSERT INTO {dvbanner_profile} (pid, name, data) VALUES (%d, '%s', '%s')",
      $pid, $form_values['name'], serialize ($data)
    );
  }
  
  // Thông báo
  drupal_set_message (t(
    "Created player profile <strong>!name</strong>.", 
    array ('!name' => check_plain ($form_values['name'])
  )));
  
  // Chuyển hướng người dùng
  drupal_goto ('admin/build/block/dvbanner/profiles');
}
