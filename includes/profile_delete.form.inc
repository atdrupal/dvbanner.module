<?php
// $Id: profile_delete.form.inc 236 2008-10-09 08:55:44Z The Hong $

/**
 * Player profile delete confirming form
 * 
 */
function dvbanner_profile_delete () {
  $pid = arg (5);
  
  // Không xoá profile mặc định
  if ($pid == 1) {
    drupal_set_message (t('You can not delete default player profile.'), 'error');
    
    $form[] = array (
      '#type' => 'markup',
      '#value' => ' '
    );  
  }
  else {
    // Kiểm tra profile có tồn tại?
    $profile = db_fetch_object (db_query (
      "SELECT pid, name FROM {dvbanner_profile} WHERE pid = %d",
      $pid
    ));
    
    if ($profile) {
      $form[] = confirm_form (
        array (
          'pid' => array (
            '#type' => 'value',
            '#value' => $pid
          )
        ), 
        t('Are you sure to delete profile <i>!name</i>?', array ('!name' => $profile->name)),
        'admin/build/block/dvbanner/profiles',
        'Nếu player profile này bị xoá đi, các player được thiết lập dựa trên profile này sẽ sử dụng profile mặc định để trình bày.'
        . ' Xin cẩn thận, thao tác này không thể hoàn hồi.'
      );
    }
    else {
      drupal_set_message (t('Profile not found.'), 'error');
      $form[] = array (
        '#type' => 'markup',
        '#value' => ' '
      );
    }
  }
  
  return $form;
}

/**
 * Validator for Player profile delete confirming form
 * 
 */
function dvbanner_profile_delete_validate ($form_id, $form_values) {

}

/**
 * Submit handler for Player profile delete confirming form
 * 
 */
function dvbanner_profile_delete_submit ($form_id, $form_values) {
  $pid = $form_values['pid'];

  // Lấy thông tin banner hiện tại, xoá các file không còn cần thiết
  $data = db_result (db_query (
    "SELECT data FROM {dvbanner_profile} WHERE pid = %d",
    $pid
  ));

  if ($data) {
    $data = unserialize($data);
    
    if ($data['image'] || $data['logo']) {
      if ($data['image']) {
        file_delete ($data['image']);
      }
      
      if ($data['logo']) {
        file_delete ($data['image']);
      }
      
      rmdir ('files/dvbanner/profiles/' . $form_values['pid']);
    }
  }
  
  // Thực hiện truy vấn
  db_query (
    "DELETE FROM {dvbanner_profile} WHERE pid = %d",
    $form_values['pid']
  );
  
  // Thông báo: đã xoá
  drupal_set_message (t("Deleted profile <strong>!name</strong>."));
  
  // Chuyển hướng người dùng
  drupal_goto ('admin/build/block/dvbanner/profiles');
}
