<?php

/**
 * Implements hook_variable_info().
 */
function content_management_variable_info($options) {
  $vars = array();

  $vars['content_management_admin_path'] = array(
    'title' => t('The target of the link to the admin interface.'),
    'type' => 'drupal_path',
    'default' => 'admin/content/node',
  );

  $vars['content_management_insert_admin_link'] = array(
    'title' => t('Add a link to the admin interface on every page.'),
    'type' => 'boolean',
    'default' => TRUE,
  );

  $vars['content_management_[node_type]_bulk_operations'] = array(
    'type' => 'multiple',
    'title' => t('Enable bulk operations', array(), $options),
    'repeat' => array(
      'type' => 'boolean',
      'default' => TRUE,
    ),
  );

  // Add global settings to content types for bulk operations.
  $options = array();
  $exclude = array(
    'action::views_bulk_operations_delete_item',
    'action::node_publish_action',
    'action::node_unpublish_action'
  );
  foreach (views_bulk_operations_get_operation_info() as $operation => $info) {
    if (!in_array($operation, $exclude)) {
      $options[$operation] = $info['label'];
    }
  }

  $vars['content_management_bulk_operations_select_[node_type]'] = array(
    'type' => 'multiple',
    'title' => t('Enable bulk operations', array(), $options),
    'repeat' => array(
      'type' => 'options',
      'options' => $options,
    ),
  );

  return $vars;
}
