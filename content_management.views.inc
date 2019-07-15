<?php

/**
 * @file
 * The content management view-file modifications.
 */

/**
 * Implements hook_views_data_alter().
 */
function content_management_views_data_alter(array &$data) {
  $data['node']['languages_edit_field'] = [
    'title' => t('Content languages'),
    'field' => [
      'title' => t('Content languages'),
      'help' => t('Get all the languages edit links.'),
      'id' => 'languages_edit_field',
    ],
  ];

  $data['node_field_data']['language_is_source_translation'] = [
    'title' => t('Is source translation'),
    'filter' => [
      'title' => t('Display only the source translation nodes'),
      'help' => t('Only show the source translation.'),
      'field' => 'langcode',
      'id' => 'language_is_source_translation',
    ],
  ];
}
