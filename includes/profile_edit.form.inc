<?php
// $Id: profile_edit.form.inc 234 2008-10-09 08:45:17Z The Hong $

/**
 * Player profile editing form
 * 
 */
function dvbanner_profile_edit () {
  // Thiết lập lại tiêu đề cho phù hợp
  drupal_set_title (t('Edit player profile'));
  
  $pid = arg (5);
  $profile = dvbanner_profile_get($pid);
  
  // Đảm bảo thông tin đã profile tồn tại 
  if (is_numeric ($pid) && $profile) {
    // Biểu mẫu có hỗ trợ upload
    $form = array (
      '#attributes' => array('enctype' => 'multipart/form-data')
    );
    
    // Profile ID
    $form['pid'] = array (
      '#type' => 'value',
      '#value' => $pid
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
    
    // Thiết lập các giá trị mặc định
    $form['name']['#default_value'] = $profile->name;
    
    if (is_array ($profile->data)) {
      foreach ($profile->data as $key => $val) {
        switch ($key) {
          case 'height':
            $form['basic_set']['height']['#default_value'] = $val;
            break;
          case 'width':
            $form['basic_set']['width']['#default_value'] = $val;
            break;
          case 'image':
            $form['basic_set']['image']['#suffix'] = theme ('image', $val, '', t('Your current image'));
            break;
          case 'backcolor':
            $form['color']['backcolor']['#default_value'] = $val;
            break;
          case 'frontcolor':
            $form['color']['frontcolor']['#default_value'] = $val;
            break;
          case 'lightcolor':
            $form['color']['lightcolor']['#default_value'] = $val;
            break;
          case 'screencolor':
            $form['color']['screencolor']['#default_value'] = $val;
            break;
          case 'logo':
            $form['display']['logo']['#suffix'] = theme ('image', $val, '', t('Your current logo'));
            break;
          case 'overstretch':
            $form['display']['overstretch']['#default_value'] = $val;
            break;
          case 'showicons':
            $form['display']['showicons']['#default_value'] = $val;
            break;
          case 'showeq':
            $form['display']['showeq']['#default_value'] = $val;
            break;
          case 'shownavigation':
            $form['control']['shownavigation']['#default_value'] = $val;
            break;
          case 'showstop':
            $form['control']['showstop']['#default_value'] = $val;
            break;
          case 'showdigits':
            $form['control']['showdigits']['#default_value'] = $val;
            break;
          case 'usefullscreen':
            $form['control']['usefullscreen']['#default_value'] = $val;
            break;
          case 'showdownload':
            $form['control']['showdownload']['#default_value'] = $val;
            break;
          case 'autostart':
            $form['playlist_behaviour']['autostart']['#default_value'] = $val;
          case 'repeat':
            $form['playlist_behaviour']['repeat']['#default_value'] = $val;
            break;
          case 'rotatetime':
            $form['playlist_behaviour']['rotatetime']['#default_value'] = $val;
            break;
          case 'shuffle':
            $form['playlist_behaviour']['shuffle']['#default_value'] = $val;
            break;
        }
      }
    }
  }
  else {
    drupal_set_message (t('Player profile not found.'), 'error');
    
    $form[] = array (
      '#type' => 'markup',
      '#value' => ''
    );  
  }
  
  // Hiển thị system tabs
  menu_set_active_item('admin/build/block/dvbanner/profiles');

  return $form;
}

/**
 * Validator for player profile editing form
 *
 */
function dvbanner_profile_edit_validate ($form_id, $form_values) {
  $pid = $form_values['pid'];
  $profile = dvbanner_profile_get($pid);
  
  // Đảm bảo thông tin đã profile tồn tại 
  if (is_numeric ($pid) && $profile) {
    // Đảm bảo tên profile không trùng lắp
    $number = db_result (db_query (
      "SELECT COUNT(*) FROM {dvbanner_profile} WHERE name = '%s' && pid != %d",
      $form_values['name'], $pid
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
    
    //// overstretch, show icon, show equalizer: đảm bảo giá trị trong tập giá trị định trước
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
  else {
    form_set_error ('', t('Profile not found.'));
  }
}

/**
 * Submit handler for player profile editing form
 * 
 */
function dvbanner_profile_edit_submit ($form_id, $form_values) {
  // Profile ID
  $pid = $form_values['pid'];
  
  $profile = dvbanner_profile_get($pid);
  
  // Nếu có tải tập tin default image/logo
  $image = file_check_upload ('image');
  $logo = file_check_upload ('logo');
  
  if ($profile->data['image']) {
    $form_values['image'] = $profile->data['image'];
  }
  
  if ($profile->data['logo']) {
    $form_values['logo'] = $profile->data['logo'];
  }
  
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
    // chỉ chấp nhận một số field xác định
    $allowed_fields = array (
      'height', 'width', 'image', 
      'backcolor', 'frontcolor', 'lightcolor', 'screencolor', 
      'logo', 'overstretch', 'showicons', 'showeq', 
      'shownavigation', 'showstop', 'showdigits', 'usefullscreen', 'showdownload',
      'autostart', 'repeat', 'shuffle', 'rotatetime'
    );
    
    if (in_array ($key, $allowed_fields)) {
      $profile->data[$key] = $val;
    }
  }
  
  // Truy vấn
  if (count ($profile->data)) {
    db_query (
      "UPDATE {dvbanner_profile} SET name = '%s', data = '%s' WHERE pid = %d",
      $form_values['name'], serialize ($profile->data), $pid
    );
  }
  
  // Thông báo
  drupal_set_message (t(
    "Updated profile <strong>!name</strong>.", 
    array ('!name' => check_plain ($form_values['name'])
  )));
  
  // Chuyển hướng người dùng
  drupal_goto ('admin/build/block/dvbanner/profiles');
}
