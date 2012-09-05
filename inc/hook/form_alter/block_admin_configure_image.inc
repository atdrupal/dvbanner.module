<?php
// $Id: block_admin_configure_image.inc 241 2008-10-09 10:07:43Z The Hong $

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

// Thêm vài kịch bản JS để quản lý banner block tiện dụng hơn
drupal_add_js (array ('dvbanner_path' => base_path () . drupal_get_path ('module', 'dvbanner')), 'setting');
drupal_add_js (drupal_get_path ('module', 'dvbanner') . '/js/block-edit.js');

// Hộp chọn tự động để chọn các banner item (type là image)
$form['block_settings']['items'] = array (
  '#type' => 'textfield',
  '#title' => 'Chọn banner item',
  '#default_value' => '',
  '#autocomplete_path' => 'dvbanner/autocomplete/image',
  '#description' => 'Nhập vào tên các banner item bạn muốn gắn thêm vào block này. <strong>Chú ý:</strong> Không hỗ trợ chọn cùng lúc nhiều item.'
);

foreach (imagecache_presets() as $pid => $preset) {
  $presetid_options[$pid] = $preset['presetname'];
}

if (!count ($presetid_options)) {
  drupal_set_message (t('Your system did not have any image preset. Please create at least one.'), 'error');
}

$form['block_settings']['displaying'] = array (
  '#type' => 'fieldset',
  '#title' => 'Kích cỡ trình bày',
  '#collapsible' => true,
  'presetid' => array (
    '#type' => 'radios',
    '#title' => t('Image preset'),
    '#options' => $presetid_options,
    '#default_value' => $block['presetid'],
	'#required' => true,
    '#description' => 'Tùy chọn này chỉ có tác dụng đối với các banner item là hình ảnh jpg, png. Hình ảnh sẽ được scale, resize, drop, ... khi hiển thị, tùy theo thiết lập của image preset.'
  ),
  'size' => array (
    '#type' => 'textfield',
    '#title' => 'Kích cỡ tùy chỉnh',
    '#default_value' => $block['size'],
    '#description' => 'Dạng nhập: HEIGHTxWIDTH. Thí dụ: 120px,86px. Bạn có thể sử dụng tùy chọn này để ràng buộc kích cỡ trình bày của các banner item trong block này.'
  ),
  'use_flash' => array (
  	'#type' => 'select',
  	'#title' => 'Sử dụng flash',
  	'#options' => array (
  	  1 => t('Yes'),
  	  0 => t('No')
  	),
  	'#default_value' => $block['use_flash'],
  	'#description' => 'Sử dụng flash player để trình bày các banner được hiển thị.'
  )
);
