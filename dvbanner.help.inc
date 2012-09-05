<?php
// $Id: dvbanner.help.inc 216 2008-10-09 07:32:42Z The Hong $

/**
 * Helping text on general issue
 *
 */
function _dvbanner_help_dvbanner () {
  $add_block_link = l(t('Add banner block'), 'admin/build/block/dvbanner/add-block');

  $output = <<<HELP
<p>\$Id: dvbanner.help.inc 216 2008-10-09 07:32:42Z The Hong $</p>
<p>
  <strong>Banner block</strong> là khối nội dung chứa các banner itemm, với số lượng banner item xác định. 
  Bạn có thể nhấp chọn tab <strong>{$add_block_link}</strong> phía trên để thêm banner block mới. 
</p>
<p>
  <strong>Chú ý:</strong>
  <ul>
    <li>Mỗi bannner block nên được đặt tên chi tiết đễ dễ dàng cho việc quản lý.
    <li>Tên của mỗi banner block là duy nhất.
    <li>Sử dụng trình duyệt web có hỗ trợ javascript khi cấu hình banner block.
  </ul>
</p>
HELP;

  return $output;
}

/**
 * Helping text on Player profile
 *
 */
function _dvbanner_help_dvbanner_profiles () {
  $add_profile_link = url('admin/build/block/dvbanner/profiles/add');

  $output = <<<HELP
<p>\$Id: dvbanner.help.inc 216 2008-10-09 07:32:42Z The Hong $</p>
<p>
  Mỗi <strong>Player profile</strong> là một tập các cấu hình trình bày video player ra giao diện người dùng. 
  Khi gắn kết flash item vào trong một banner block, bạn có thể chọn lựa các player profile khác nhau cho mỗi flash item.
  Đối với các flash item không có player profile xác định, hệ thống sẽ sử dụng player profile mặc định. 
</p>
<p>
  Bạn có thể tạo <a href="{$add_profile_link}">player profile</a> mới, hoặc chọn lựa các thao tác ứng với các  các player profle
  sau đây.
</p>
<p>
  <strong>Chú ý:</strong> Không thể xoá player profile mặc định.
</p>
HELP;

  return $output;
}
