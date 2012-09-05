<?php
// $Id: block_admin_configure_submit.inc 526 2008-11-28 09:28:32Z The Hong $

$delta = $form_values['delta'];

// Remove XML cached file
file_delete (file_directory_path () . "/dvbanner/playlist/{$delta}.xml");

// Biến mảng lưu trữ các banner items để lưu
$items = array ();

// Tìm profile ID từ profile name đã nhập vào
$profile_id = db_result (db_query (
  "SELECT pid FROM {dvbanner_profile} WHERE name = '%s'",
  $form_values['profile']
));

// Tạm thời xoá bỏ tất cả các banner items hiện tại của block
db_query (
	"DELETE FROM {dvbanner_block_items} WHERE delta = %d",
	$delta
);

if ($form_values['type'] == 'flv') {
	foreach ($form_values['current_items'] as $bid => $val) {
		// Không lưu các item không được chọn
		if ($bid != $val) {
			continue;
		}

		// Banner item đã tồn tại từ trước
		if (is_numeric ($bid)) {
			$items[$bid] = $bid;
		}
		// Banner item mới được thêm vào
		else {
			// thay thế tên item thành mã số
			$bid = db_result (db_query (
				"SELECT bid FROM {dvbanner_item} WHERE title = '%s'", $val
			));

			// Chắc chắn là đã tìm thấy banner item trong CSDL
			if (is_numeric ($bid)) {
				$items[$bid] = $bid;
			}
		}
	}

	// Lưu vào thông tin của block
	db_query (
		"UPDATE {dvbanner_block} SET title = '%s', pid = %d, data = '%s' WHERE delta = %d",
		$form_values['title'],
		$profile_id,
		serialize (array (
			'limitation' => $form_values['limitation']
		)),
		$delta
	);
}
elseif ($form_values['type'] == 'image') {
  foreach ($form_values['current_items'] as $bid => $val) {
    // Không lưu các item không được chọn
    if ($bid != $val) {
      continue;
    }

    // Banner item đã tồn tại từ trước
    if (is_numeric ($bid)) {
      $items[$bid] = $bid;
    }
    // Banner item mới được thêm vào
    else {
      // thay thế tên item thành mã số
      $bid = db_result (db_query (
        "SELECT bid FROM {dvbanner_item} WHERE title = '%s'", $val
      ));

      // Chắc chắn là đã tìm thấy banner item trong CSDL
      if (is_numeric ($bid)) {
        $items[$bid] = $bid;
      }
    }
  }


//	// Lại thêm vào các banner đang tồn tại trong block
//	if (isset($form_values['current_items']) && is_array($form_values['current_items'])) {
//	  foreach ($form_values['current_items'] as $key => $val) {
//	    if ($val !== 0) {
//	      $items[] = $key;
//	    }
//	  }
//	}
//
//	// Các item mới
//	if (!empty ($form_values['items'])) {
//	  $form_values['items'] = explode (',', $form_values['items']);
//
//	  foreach ($form_values['items'] as $title) {
//	  	// Từ tiêu đề, chọn banner item ID tương ứng
//	    $bid = db_result (db_query (
//	    	"SELECT bid FROM {dvbanner_item} WHERE title = '%s'",
//	      trim ($title)
//	    ));
//
//	    if (is_numeric ($bid)) {
//	      $items[$bid] = $bid;
//	    }
//	  }
//	}

	// Lưu vào thông tin của block
	$data = array ();
	$data['presetid'] = $form_values['presetid'];
	$data['limitation'] = $form_values['limitation'];
	if (!empty($form_values['size'])) {
		$data['size'] = $form_values['size'];
	}

	$data['use_flash'] = $form_values['use_flash'];

	db_query (
		"UPDATE {dvbanner_block} SET title = '%s', pid = %d, data = '%s' WHERE delta = %d",
		$form_values['title'],
		$profile_id,
		serialize ($data),
		$delta
	);
}

// Thực hiện các truy vấn lưu trữ
foreach ($items as $item) {
	db_query (
		"INSERT INTO {dvbanner_block_items} (delta, bid) VALUES (%d, %d)",
		$delta, $item
	);
}
