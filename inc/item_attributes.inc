<?php
// $Id: item_attributes.inc 207 2008-10-09 05:05:32Z The Hong $

$attributes['title'] = array (
  '#title' => t('Title'),
  '#type' => 'textfield',
  '#required' => true,
  '#description' => 'Every banner item should have a title for easier to manage.'
);

$attributes['#attributes'] = array('enctype' => 'multipart/form-data');
$attributes['resource'] = array (
  '#type' => 'fieldset',
  '#title' => 'Resource',
  '#collapsible' => true
);

$attributes['resource']['path'] = array (
  '#title' => 'Path',
  '#type' => 'textfield',
  '#description' => 'Enter path to your banner if you put it somewhere in the internet.'
);

$attributes['resource']['file'] = array (
  '#type' => 'file',
  '#title' => t('File'),
  '#description' => 'Only allow media file types (gif, jpeg, jpg, bmp, png, flv, wmv, ...). If there is entered some file to this upload field, this value will override the path field.'
);

$attributes['resource']['link'] = array (
  '#type' => 'textfield',
  '#title' => 'Link to',
  '#description' => t('Enter web page you would like to link your customer to when they click the banner. Exampe: www.example.com')
);

$attributes['displaying'] = array (
  '#type' => 'fieldset',
  '#title' => 'Displaying',
	'#collapsible' => true
);

$attributes['displaying']['status'] = array (
  '#type' => 'checkbox',
  '#title' => t('Active'),
  '#default_value' => 1,
  '#description' => t('If this field un-checked, the banner will be unavailble for any banner block.'),
);

$attributes['displaying']['weight'] = array (
  '#type' => 'weight',
  '#title' => t('Weight'),
  '#description' => t('Optional. In the menu, the heavier items will sink and the lighter items will be positioned nearer the top.')
);
