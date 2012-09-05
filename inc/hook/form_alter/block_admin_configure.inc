<?php
// $Id: block_admin_configure.inc 241 2008-10-09 10:07:43Z The Hong $

$delta = $form['delta']['#value'];

// Gắn cứng 100 ở đây hơi dở, rảnh thì làm lại cho đẹp hơn
$block = dvbanner_block_get_info ($delta, 100);

// Thay đổi block title mặc định lấy từ bảng dvbanne_block   
$form['block_settings']['title']['#default_value'] = $block['title'];

if (in_array ($block['type'], array ('image', 'flv'))) {
	// Thêm các field tuỳ theo banner type
	require dirname (__FILE__) . '/block_admin_configure_' . $block['type'] . '.inc';

	// Tuỳ chọn giới hạn số lượng trình bày
	$form['block_settings']['limitation'] = array (
	  '#type' => 'select',
	  '#title' => 'Giới hạn',
	  '#options' =>  drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 25, 30)),
	  '#default_value' => $block['limitation'],
	  '#description' => 'Bạn có thể thiết lập số lượng banner item tối đa có thể hiển thị cùng lúc trong block này.'
			. '<strong>Chú ý:</strong> Mỗi lần trình bày, banner block chỉ hiển thị số banner đúng bằng giá trị đã thiết lập ở trường dữ liệu này.',
	  '#weight' => -1
	);
	
	// Thêm block type field vào biểu mẫu để các kịch bản sau dễ xử lý
	$form['block_settings']['type'] = array (
		'#type' => 'value',
		'#value' => $block['type']
	);
	
	// Hộp chọn player profile
	$form['block_settings']['profile'] = array (
		'#type' => 'textfield',
		'#title' => t('Player profile'),
		'#autocomplete_path' => 'dvbanner/autocomplete/profile',
		'#default_value' => db_result (db_query (
		  "SELECT name FROM {dvbanner_profile} WHERE pid = %d",
      $block['pid']
    )),
		'#description' => 
		  'Chọn player profile bạn muốn sử dụng để trình bày các banner item.'
		  . ' Nếu để trống, banner block sẽ sử dụng profile mặc định.'
			. '<strong>Chú ý:</strong> Profile chỉ có được sử dụng khi banner block có từ 2 banner item trở lên.'
	);
	
	// Thêm một nút nhấn để tiện cho người dùng
	$form['block_settings']['button'] = array (
		'#type' => 'submit',
		'#value' => t('Save block')
	);
	
	$form['user_vis_settings']['#collapsed'] = true;
	$form['role_vis_settings']['#collapsed'] = true;
	$form['page_vis_settings']['#collapsed'] = true;
	
	// Thêm kịch bản xử lý tuỳ chỉnh
	$form['#submit']['_dvbanner_forms_submit'] = array ();
}

// Hiển thị system tabs
menu_set_active_item('admin/build/block/dvbanner');
