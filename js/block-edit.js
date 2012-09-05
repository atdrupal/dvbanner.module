// $Id: block-edit.js 207 2008-10-09 05:05:32Z The Hong $

$(function(){
	// Hide this select box if there is no option
	if ($('#edit-current-items option').size() < 1) {
		$('#edit-current-items').hide();
	}
	
	// Add "Add" image button after "add banner item" field
	$('#edit-items').after('<img id="edit-items-add" src="'+ Drupal.settings.dvbanner_path + '/images/add.gif" />');
	
	// Add click handler for add item image button
	$('#edit-items-add').click(function(){
		dvbanner_add_item ($('#edit-items').val());
	});
});

function dvbanner_add_item (str) {
	var _existed = false;
	
	str = $.trim(str);
	
	// Empty add items field
	$('#edit-items').val('');
	
	// Is this item already existed
	$('#edit-current-items option').each(function(){
		if ($(this).html() == str) {
			_existed = true;
		}
	});
	
	if (!_existed) {
		// Append item to current items field, run simple fade out effect
		$('#edit-current-items')
			.append('<option value="' + str + '">' + str + '</option>')
			.fadeIn('slow');
	}
}
