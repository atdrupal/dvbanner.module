<?php
// $Id: profile_attributes.inc 207 2008-10-09 05:05:32Z The Hong $

//////////////////////////////////
// Add Farbtastic color picker
drupal_add_css('misc/farbtastic/farbtastic.css', 'module', 'all', FALSE);
drupal_add_js('misc/farbtastic/farbtastic.js');

drupal_add_js ("$(function(){
  $('#colorpicker').css('float', 'right');
  var f = $.farbtastic('#colorpicker');
  var p = $('#picker').css('opacity', 0.25);
  var selected;
    
  $('.colorwell').css({'cursor': 'pointer'})
    .each(function (){
      f.linkTo(this); $(this).css('opacity', 0.75);
    })
    .focus(function(){
      if (selected) {
        $(selected).css('opacity', 0.75);
	   }
	   
	   f.linkTo(this);
	   p.css('opacity', 1);
	   $(selected = this).css('opacity', 1);
    });
  });", 'inline');

$bool_no = array (
  'false' => t('No (default)'),
  'true' => t('Yes')
);

$bool_yes = array (
  'true' => t('Yes (default)'),
  'false' => t('No')
);

//////////////////////////////////
// Basic settings
$attributes['basic_set'] = array (
  '#type' => 'fieldset',
  '#title' => t('Basic settings'),
  '#description' => '@TODO: Nội dung trợ giúp cho basic settings.',
  '#collapsible' => true,
//  '#collapsed' => true
);

$attributes['basic_set']['height'] = array (
  '#type' => 'textfield',
  '#title' => t('Height'),
  '#size' => 3,
  '#default_value' => '250',
  '#field_suffix' => 'px',
	'#required' => true,
  '#description' => t('The overall height of the player/rotator.')
);

$attributes['basic_set']['width'] = array (
  '#type' => 'textfield',
  '#title' => t('Width'),
  '#size' => 3,
  '#default_value' => '400',
  '#field_suffix' => 'px',
	'#required' => true,
	'#description' => t('The overall width of the player/rotator.')
);

$attributes['basic_set']['image'] = array (
  '#type' => 'file',
  '#title' => t('Image'),
  '#description' => t('Image to be displayed when flash content has not played.')
);

//////////////////////////////////
// Color settings
$attributes['color'] = array (
  '#type' => 'fieldset',
  '#title' => t('Color settings'),
  '#description' => '@TODO: Nội dung trợ giúp cho color settings.',
  '#collapsible' => true,
//  '#collapsed' => true
);

$attributes['color']['colorpicker'] = array (
  '#type' => 'markup',
  '#value' => '<div id="colorpicker"></div>'
);

$attributes['color']['backcolor'] = array (
  '#type' => 'textfield',
  '#title' => t('Background color'),
  '#description' => t('Background color of control bar.'),
  '#default_value' => '#FFFFFF',
  '#attributes' => array ('class' => 'colorwell'),
  '#size' => 7
);

$attributes['color']['frontcolor'] = array (
  '#type' => 'textfield',
  '#title' => t('Front color'),
  '#default_value' => '#000000',
  '#description' => t('Text and button color of control bar.'),
  '#attributes' => array ('class' => 'colorwell'),
  '#size' => 7
);

$attributes['color']['lightcolor'] = array (
  '#type' => 'textfield',
  '#title' => t('Hover color'),
  '#default_value' => '#000000',
  '#description' => t('Rollover color of control.'),
  '#attributes' => array ('class' => 'colorwell'),
  '#size' => 7
);

$attributes['color']['screencolor'] = array (
  '#type' => 'textfield',
  '#title' => t('Screen color'),
  '#default_value' => '#000000',
  '#description' => t('Background color of display area.'),
  '#attributes' => array ('class' => 'colorwell'),
  '#size' => 7
);

//////////////////////////////////
// Displaying settings
$attributes['display'] = array (
  '#type' => 'fieldset',
  '#title' => t('Displaying settings'),
  '#description' => '@TODO: Nội dung trợ giúp cho display settings.',
  '#collapsible' => true,
//  '#collapsed' => true
);

$attributes['display']['logo'] = array (
  '#type' => 'file',
  '#title' => t('Logo')
);

$attributes['display']['overstretch'] = array (
  '#type' => 'select',
  '#title' => t('Overstretch'),
  '#options' => array (
    'false' => t('No (default)'),
    'true' => t('Yes'),
    'fit' => t('Fit'),
    'none' => t('None')
  ),
  '#description' => t('Set how to stretch image and movie to fix the display')
);

$attributes['display']['showicons'] = array (
  '#type' => 'select',
  '#title' => t('Show icons'),
  '#options' => $bool_yes,
  '#description' => t('Set this to false to hide the activity icon and play button in the display.')
);

$attributes['display']['showeq'] = array (
  '#type' => 'select',
  '#title' => t('Show equalizer'),
  '#options' => $bool_no,
  '#description' => t('Show an equalizer in the display (for MP3)')
);

//////////////////////////////////
// Controlbar appearance
$attributes['control'] = array (
  '#type' => 'fieldset',
  '#title' => t('Control bar display appearance'),
  '#description' => '@TODO: Nội dung trợ giúp cho display settings.',
  '#collapsible' => true,
//  '#collapsed' => true
);

$attributes['control']['shownavigation'] = array (
  '#type' => 'select',
  '#title' => t('Show navigation'),
  '#options' => $bool_yes,
  '#description' => t('Set this to false to disable the controlbar.')
);

$attributes['control']['showstop'] = array (
  '#type' => 'select',
  '#title' => t('Show stop'),
  '#options' => $bool_yes,
  '#description' => t('Set this to true to show a stop button in the controlbar.')
);

$attributes['control']['showdigits'] = array (
  '#type' => 'select',
  '#title' => t('Show digits'),
  '#options' => $bool_yes,
  '#description' => t('Set this to false to hide the elapsed/remaining digits in the controlbar.')
);

$attributes['control']['usefullscreen'] = array (
  '#type' => 'select',
  '#title' => t('Use fullscreen'),
  '#options' => $bool_yes,
  '#description' => t('Set this to false to disable fullscreen.')
);

$attributes['control']['showdownload'] = array (
  '#type' => 'select',
  '#title' => t('Show download'),
  '#options' => $bool_yes,
  '#description' => t('Set to true to show a link button in the navigation bar.')
);

//////////////////////////////////
// Playlist appearance

//////////////////////////////////
// Playlist behavior
$attributes['playlist_behaviour'] = array (
  '#type' => 'fieldset',
  '#title' => t('Playlist behaviour'),
  '#collapsible' => true,
  // '#collapsed' => true
);

$attributes['playlist_behaviour']['autostart'] = array (
  '#type' => 'select',
  '#title' => t('Auto start'),
  '#options' => $bool_no,
  '#description' => t('Set to true to automatically start playing on page load')
);

$attributes['playlist_behaviour']['repeat'] = array (
  '#type' => 'select',
  '#title' => t('Repeat'),
  '#options' => array (
    'false' => t('No (default)'),
    'list' => t('List'),
    'true' => t('Yes')
  ),
  '#description' => t('Set to true to repeat playback and list to playback the entire playlist once.')
);

$attributes['playlist_behaviour']['rotatetime'] = array (
  '#type' => 'textfield',
  '#title' => t('Rotate time (second)'),
  '#description' => t('Set this to false to playback the playlist sequentially instead of shuffled.')
);

$attributes['playlist_behaviour']['shuffle'] = array (
  '#type' => 'select',
  '#title' => t('Shuffle'),
  '#options' => $bool_no,
  '#description' => t('Set this to false to playback the playlist sequentially instead of shuffled.')
);
