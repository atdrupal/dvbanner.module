<?php
// $Id: block_admin_configure_flv.inc 207 2008-10-09 05:05:32Z The Hong $

// Thêm vài kịch bản JS để quản lý banner block tiện dụng hơn
drupal_add_js (array ('dvbanner_path' => base_path () . drupal_get_path ('module', 'dvbanner')), 'setting');
drupal_add_js (drupal_get_path ('module', 'dvbanner') . '/js/block-edit.js');

// Liệt kê các banner items hiện tồn tại trong banner block này
if (count ($block['items'])) {
	foreach ($block['items'] as $item) {
		$current_items[$item->bid] = $item->title;
	}
}

$form['block_settings']['current_items'] = array (
  '#type' => 'select',
  '#multiple' => true,
  '#title' => t('Current banner items'),
  '#default_value' => count ($current_items) ? array_keys ($current_items) : array (),
  '#options' => count($current_items) ? $current_items : array (),
  '#description' => 'Bỏ chọn các banner item để bỏ chúng khỏi block này.',
	'#weight' => -2
);

// Hộp chọn tự động để chọn các banner item (type là image)
$form['block_settings']['items'] = array (
  '#type' => 'textfield',
  '#title' => 'Chọn banner item',
  '#default_value' => '',
  '#autocomplete_path' => 'dvbanner/autocomplete/flv',
	'#description' => 'Nhập vào tên các banner item bạn muốn gắn thêm vào block này. <strong>Chú ý:</strong> Không hỗ trợ chọn cùng lúc nhiều item.'
);
