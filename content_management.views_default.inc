<?php
/**
 * @file
 * Content management default views.
 */

/**
 * Implements hook_views_default_views().
 */
function content_management_views_default_views() {
  $node_types = node_type_get_types();

  $export = array();

  /* Dynamically add bulk operations based on the content type */
  foreach ($node_types as $node_type) {
    $enable_bulk_operations = variable_get_value('content_management_' . $node_type->type . '_bulk_operations');
    $enabled_bulk_operations = array();
    if ($enable_bulk_operations) {
      $enabled_bulk_operations = variable_get_value('content_management_bulk_operations_select_' . $node_type->type);
    }
    $weight = FALSE;
    if (module_exists('weight')) {
      $type_sets = _weight_get_settings($node_type->type);
      if (isset($type_sets['enabled']) && $type_sets['enabled']) {
        $weight = TRUE;
      }
    }

    /*
     * View 'content_management'
     */
    $view = new view();
    $view->name = 'cm_' . $node_type->type;
    $view->description = '';
    $view->tag = 'Content management';
    $view->base_table = 'node';
    $view->human_name = '';
    $view->core = 0;
    $view->api_version = '3.0';
    $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */


    /* Display: Defaults */
    $handler = $view->new_display('default', 'Defaults', 'default');
    $handler->display->display_options['title'] = "Manage {$node_type->name} content";
    $handler->display->display_options['css_class'] = 'content-management-view';
    $handler->display->display_options['use_more_text'] = 'more';
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['access']['perm'] = "manage {$node_type->type} content";
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['query']['options']['query_comment'] = FALSE;
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Reset';
    $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sort by';
    $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Descending';
    $handler->display->display_options['pager']['type'] = 'full';
    $handler->display->display_options['pager']['options']['items_per_page'] = 25;
    $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Items per page';
    $handler->display->display_options['style_plugin'] = 'table';

    $columns = array();
    if ($enable_bulk_operations) {
      $columns['views_bulk_operations'] = 'views_bulk_operations';
    }

    $columns += array(
      'title' => 'title',
      'timestamp' => 'title',
      'operations' => 'operations',
      'status' => 'status',
    );

    if ($weight) {
      $columns += array(
        'weight_value' => 'weight_value',
        'weight' => 'weight',
      );
    }

    $handler->display->display_options['style_options']['columns'] = $columns;
    $handler->display->display_options['style_options']['default'] = '-1';
    $handler->display->display_options['style_options']['info'] = array(
      'views_bulk_operations' => array(
        'align' => '',
        'separator' => '',
      ),
      'title' => array(
        'sortable' => 1,
        'default_sort_order' => 'asc',
        'align' => '',
        'separator' => '',
      ),
      'timestamp' => array(
        'align' => '',
        'separator' => '',
      ),
      'operations' => array(
        'align' => '',
        'separator' => '',
      ),
      'status' => array(
        'sortable' => 1,
        'default_sort_order' => 'asc',
        'align' => '',
        'separator' => '',
      ),
    );
    if ($weight) {
      $handler->display->display_options['style_options']['info']['weight_value'] = array(
        'sortable' => 1,
        'default_sort_order' => 'asc',
        'align' => '',
        'separator' => '',
        'empty_column' => 0,
      );
      $handler->display->display_options['style_options']['info']['weight'] = array(
        'sortable' => 1,
        'default_sort_order' => 'asc',
        'align' => '',
        'separator' => '',
        'empty_column' => 0,
      );
    }

    /* Cycle through all fields of this node type to see which ones will be added and will require a sortable column */
    foreach (field_info_instances('node', $node_type->type) as $key => $val) {
      if (variable_get($node_type->type . '_' . $key . '_add_as_column_in_cm', 0) && variable_get($node_type->type . '_' . $key . '_add_column_cm_sort', 0)) {
        $handler->display->display_options['style_options']['info'][$key] = array(
          'sortable' => 1,
          'default_sort_order' => 'asc',
          'align' => '',
          'separator' => '',
          'empty_column' => 0,
        );
      }
    }

    $handler->display->display_options['style_options']['override'] = 1;
    $handler->display->display_options['style_options']['sticky'] = 0;
    $handler->display->display_options['style_options']['empty_table'] = 0;

    if (module_exists('locale') && locale_multilingual_node_type($node_type->type) && module_exists('i18n_node')) {
      /* Header: Global: Unfiltered text */
      $handler->display->display_options['header']['area_text_custom']['id'] = 'area_text_custom';
      $handler->display->display_options['header']['area_text_custom']['table'] = 'views';
      $handler->display->display_options['header']['area_text_custom']['field'] = 'area_text_custom';
      $handler->display->display_options['header']['area_text_custom']['content'] = '<div id="cm-legend"><b><u>Legend</u></b><br /><span class="marker">*</span> = New node<br /> <span class="marker">!</span> = Recently updated node</div>';
    }
    /* Field: Content: Bulk operations */
    if ($enable_bulk_operations) {
      $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
      $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
      $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['alter_text'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['make_link'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['absolute'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['external'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['replace_spaces'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['trim_whitespace'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['nl2br'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['word_boundary'] = 1;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['ellipsis'] = 1;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['strip_tags'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['trim'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['alter']['html'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = 1;
      $handler->display->display_options['fields']['views_bulk_operations']['element_default_classes'] = 1;
      $handler->display->display_options['fields']['views_bulk_operations']['hide_empty'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['empty_zero'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '1';
      $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
      $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
      $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
      $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
        'action::views_bulk_operations_delete_item' => array(
          'selected' => 1,
          'postpone_processing' => 0,
          'skip_confirmation' => 0,
          'override_label' => 0,
          'label' => '',
        ),
        'action::node_publish_action' => array(
          'selected' => 1,
          'postpone_processing' => 0,
          'skip_confirmation' => 0,
          'override_label' => 0,
          'label' => '',
        ),
        'action::node_unpublish_action' => array(
          'selected' => 1,
          'postpone_processing' => 0,
          'skip_confirmation' => 0,
          'override_label' => 0,
          'label' => '',
        ),
      );
      // Add extra bulk operations if they are selected on the content type settings form.
      if ($enable_bulk_operations && count($enabled_bulk_operations) > 0) {
        foreach ($enabled_bulk_operations as $operation) {
          $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'][$operation] = array(
            'selected' => 1,
            'postpone_processing' => 0,
            'skip_confirmation' => 0,
            'override_label' => 0,
            'label' => '',
          );
        }
      }
    }

    /* Field: Content: Title */
    $handler->display->display_options['fields']['title']['id'] = 'title';
    $handler->display->display_options['fields']['title']['table'] = 'node';
    $handler->display->display_options['fields']['title']['field'] = 'title';
    $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['title']['alter']['html'] = 0;
    $handler->display->display_options['fields']['title']['hide_empty'] = 0;
    $handler->display->display_options['fields']['title']['empty_zero'] = 0;
    $handler->display->display_options['fields']['title']['link_to_node'] = 1;
    /* Field: Content: Has new content */
    $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
    $handler->display->display_options['fields']['timestamp']['table'] = 'history';
    $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
    $handler->display->display_options['fields']['timestamp']['label'] = 'New?';
    $handler->display->display_options['fields']['timestamp']['link_to_node'] = 0;
    $handler->display->display_options['fields']['timestamp']['comments'] = 0;
    /* Field: Content: Published */
    $handler->display->display_options['fields']['status']['id'] = 'status';
    $handler->display->display_options['fields']['status']['table'] = 'node';
    $handler->display->display_options['fields']['status']['field'] = 'status';
    $handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['status']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['status']['alter']['html'] = 0;
    $handler->display->display_options['fields']['status']['hide_empty'] = 0;
    $handler->display->display_options['fields']['status']['empty_zero'] = 0;
    $handler->display->display_options['fields']['status']['not'] = 0;
    if (module_exists('locale') && locale_multilingual_node_type($node_type->type) && module_exists('i18n_node')) {
      /* Field: Node translation: Translation controls */
      $handler->display->display_options['fields']['translations']['id'] = 'translations';
      $handler->display->display_options['fields']['translations']['table'] = 'node';
      $handler->display->display_options['fields']['translations']['field'] = 'translations';
      $handler->display->display_options['fields']['translations']['label'] = 'Translations';
      $handler->display->display_options['fields']['translations']['alter']['alter_text'] = 0;
      $handler->display->display_options['fields']['translations']['alter']['make_link'] = 0;
      $handler->display->display_options['fields']['translations']['alter']['absolute'] = 0;
      $handler->display->display_options['fields']['translations']['alter']['external'] = 0;
      $handler->display->display_options['fields']['translations']['alter']['replace_spaces'] = 0;
      $handler->display->display_options['fields']['translations']['alter']['trim_whitespace'] = 0;
      $handler->display->display_options['fields']['translations']['alter']['nl2br'] = 0;
      $handler->display->display_options['fields']['translations']['alter']['word_boundary'] = 1;
      $handler->display->display_options['fields']['translations']['alter']['ellipsis'] = 1;
      $handler->display->display_options['fields']['translations']['alter']['strip_tags'] = 0;
      $handler->display->display_options['fields']['translations']['alter']['trim'] = 0;
      $handler->display->display_options['fields']['translations']['alter']['html'] = 0;
      $handler->display->display_options['fields']['translations']['element_label_colon'] = 1;
      $handler->display->display_options['fields']['translations']['element_default_classes'] = 1;
      $handler->display->display_options['fields']['translations']['hide_empty'] = 0;
      $handler->display->display_options['fields']['translations']['empty_zero'] = 0;
      $handler->display->display_options['fields']['translations']['hide_alter_empty'] = 0;
    }
    /* Field: Content: Operations */
    $handler->display->display_options['fields']['operations']['id'] = 'operations';
    $handler->display->display_options['fields']['operations']['table'] = 'node';
    $handler->display->display_options['fields']['operations']['field'] = 'operations';
    $handler->display->display_options['fields']['operations']['alter']['alter_text'] = 0;
    $handler->display->display_options['fields']['operations']['alter']['make_link'] = 0;
    $handler->display->display_options['fields']['operations']['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields']['operations']['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields']['operations']['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields']['operations']['alter']['trim'] = 0;
    $handler->display->display_options['fields']['operations']['alter']['html'] = 0;
    $handler->display->display_options['fields']['operations']['hide_empty'] = 0;
    $handler->display->display_options['fields']['operations']['empty_zero'] = 0;

    /* Cycle through all fields of this node type to find extra added columns */
    foreach (field_info_instances('node', $node_type->type) as $key => $val) {
      if (variable_get($node_type->type . '_' . $key . '_add_as_column_in_cm', 0)) {
        $field_info = field_info_field($key);
        $handler->display->display_options['fields'][$key]['id'] = $key;
        $handler->display->display_options['fields'][$key]['table'] = 'field_data_' . $key;
        $handler->display->display_options['fields'][$key]['field'] = $key;
        // Add extra styling based on the field type.
        switch ($field_info['type']) {
          case 'image':
            $handler->display->display_options['fields'][$key]['click_sort_column'] = 'fid';
            $handler->display->display_options['fields'][$key]['settings'] = array(
              'image_style' => 'thumbnail',
              'image_link' => '',
            );
            break;

          case 'text_long':
            $handler->display->display_options['fields'][$key]['type'] = 'text_trimmed';
            $handler->display->display_options['fields'][$key]['settings'] = array(
              'trim_length' => '50',
            );
            break;
        }
      }
    }


    if ($weight) {
      /* Field: Weight: Weight */
      $handler->display->display_options['fields']['weight']['id'] = 'weight';
      $handler->display->display_options['fields']['weight']['table'] = 'weight_weights';
      $handler->display->display_options['fields']['weight']['field'] = 'weight';
      /* Field: Weight: Weight value */
      $handler->display->display_options['fields']['weight_value']['id'] = 'weight_value';
      $handler->display->display_options['fields']['weight_value']['table'] = 'weight_weights';
      $handler->display->display_options['fields']['weight_value']['field'] = 'weight';
    }

    if ($weight) {
      /* Sort criterion: Weight: Weight */
      $handler->display->display_options['sorts']['weight']['id'] = 'weight_value';
      $handler->display->display_options['sorts']['weight']['table'] = 'weight_weights';
      $handler->display->display_options['sorts']['weight']['field'] = 'weight';
      $handler->display->display_options['sorts']['weight']['order'] = 'ASC';
    }
    /* Sort criterion: Content: Updated date */
    $handler->display->display_options['sorts']['changed']['id'] = 'changed';
    $handler->display->display_options['sorts']['changed']['table'] = 'node';
    $handler->display->display_options['sorts']['changed']['field'] = 'changed';
    $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
    /* Contextual filter: Content: Type */
    $handler->display->display_options['arguments']['type']['id'] = 'type';
    $handler->display->display_options['arguments']['type']['table'] = 'node';
    $handler->display->display_options['arguments']['type']['field'] = 'type';
    $handler->display->display_options['arguments']['type']['exception']['title_enable'] = 1;
    $handler->display->display_options['arguments']['type']['default_argument_type'] = 'fixed';
    $handler->display->display_options['arguments']['type']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['type']['specify_validation'] = 1;

    /* Filter criterion: Content: Title */
    $handler->display->display_options['filters']['title']['id'] = 'title';
    $handler->display->display_options['filters']['title']['table'] = 'node';
    $handler->display->display_options['filters']['title']['field'] = 'title';
    $handler->display->display_options['filters']['title']['operator'] = 'contains';
    $handler->display->display_options['filters']['title']['exposed'] = TRUE;
    $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
    $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
    $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
    $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
    /* Filter criterion: Content: Published */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'node';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = 'All';
    $handler->display->display_options['filters']['status']['exposed'] = TRUE;
    $handler->display->display_options['filters']['status']['expose']['label'] = 'Published';
    $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
    $handler->display->display_options['filters']['status']['expose']['required'] = FALSE;
    /* Filter criterion: Content: Type */
    $handler->display->display_options['filters']['type']['id'] = 'type';
    $handler->display->display_options['filters']['type']['table'] = 'node';
    $handler->display->display_options['filters']['type']['field'] = 'type';
    $handler->display->display_options['filters']['type']['value'] = array(
      $node_type->type => $node_type->type,
    );
    $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;


    /* Cycle through all fields of this node type to see which ones will be added as a filter */
    foreach (field_info_instances('node', $node_type->type) as $key => $val) {
      if (variable_get($node_type->type . '_' . $key . '_cm_use_as_exposed_filter', 0)) {
        $field_info = field_info_field($key);
        switch ($field_info['type']) {
          case 'text_long':
            $handler->display->display_options['filters'][$key . '_value']['id'] = $key . '_value';
            $handler->display->display_options['filters'][$key . '_value']['table'] = 'field_data_' . $key;
            $handler->display->display_options['filters'][$key . '_value']['field'] = $key . '_value';
            $handler->display->display_options['filters'][$key . '_value']['operator'] = 'contains';
            $handler->display->display_options['filters'][$key . '_value']['exposed'] = TRUE;
            $handler->display->display_options['filters'][$key . '_value']['expose']['operator_id'] = $key . '_value_op';
            $handler->display->display_options['filters'][$key . '_value']['expose']['label'] = $val['label'];
            $handler->display->display_options['filters'][$key . '_value']['expose']['operator'] = $key . '_value_op';
            $handler->display->display_options['filters'][$key . '_value']['expose']['identifier'] = $key . '_value';
            $handler->display->display_options['filters'][$key . '_value']['expose']['remember_roles'] = array(
              2 => '2',
              1 => 0,
              3 => 0,
              4 => 0,
            );

            break;
          case 'text':
            $handler->display->display_options['filters'][$key . '_value']['id'] = $key . '_value';
            $handler->display->display_options['filters'][$key . '_value']['table'] = 'field_data_' . $key;
            $handler->display->display_options['filters'][$key . '_value']['field'] = $key . '_value';
            $handler->display->display_options['filters']['field_label_key_value']['operator'] = 'contains';
            $handler->display->display_options['filters']['field_label_key_value']['exposed'] = TRUE;
            $handler->display->display_options['filters'][$key . '_value']['expose']['operator_id'] = $key . '_value_op';
            $handler->display->display_options['filters'][$key . '_value']['expose']['label'] = $val['label'];
            $handler->display->display_options['filters'][$key . '_value']['expose']['operator'] = $key . '_value_op';
            $handler->display->display_options['filters'][$key . '_value']['expose']['identifier'] = $key . '_value';
            $handler->display->display_options['filters'][$key . '_value']['expose']['remember_roles'] = array(
              2 => '2',
              1 => 0,
              3 => 0,
              4 => 0,
            );
            break;

          case 'node_reference':
            $handler->display->display_options['filters'][$key . '_nid']['id'] = $key . '_nid';
            $handler->display->display_options['filters'][$key . '_nid']['table'] = 'field_data_' . $key;
            $handler->display->display_options['filters'][$key . '_nid']['field'] = $key . '_nid';
            $handler->display->display_options['filters'][$key . '_nid']['exposed'] = TRUE;
            $handler->display->display_options['filters'][$key . '_nid']['expose']['operator_id'] = $key . '_nid_op';
            $handler->display->display_options['filters'][$key . '_nid']['expose']['label'] = $val['label'];
            $handler->display->display_options['filters'][$key . '_nid']['expose']['operator'] = $key . '_nid_op';
            $handler->display->display_options['filters'][$key . '_nid']['expose']['identifier'] = $key . '_nid';
            if (variable_get($node_type->type . '_' . $key . '_cm_exposed_filter_multivalue', 0)) {
              $handler->display->display_options['filters'][$key . '_nid']['expose']['multiple'] = TRUE;
            }
            $handler->display->display_options['filters'][$key . '_nid']['expose']['remember_roles'] = array(
              2 => '2',
              1 => 0,
              3 => 0,
              4 => 0,
            );
            break;

          case 'taxonomy_term_reference':
            $handler->display->display_options['filters'][$key . '_tid']['id'] = $key . '_tid';
            $handler->display->display_options['filters'][$key . '_tid']['table'] = 'field_data_' . $key;
            $handler->display->display_options['filters'][$key . '_tid']['field'] = $key . '_tid';
            $handler->display->display_options['filters'][$key . '_tid']['exposed'] = TRUE;
            $handler->display->display_options['filters'][$key . '_tid']['expose']['operator_id'] = $key . '_tid_op';
            $handler->display->display_options['filters'][$key . '_tid']['expose']['label'] = $val['label'];
            $handler->display->display_options['filters'][$key . '_tid']['expose']['operator'] = $key . '_tid_op';
            $handler->display->display_options['filters'][$key . '_tid']['expose']['identifier'] = $key . '_tid';
            if (variable_get($node_type->type . '_' . $key . '_cm_exposed_filter_multivalue', 0)) {
              $handler->display->display_options['filters'][$key . '_nid']['expose']['multiple'] = TRUE;
            }
            $handler->display->display_options['filters'][$key . '_tid']['expose']['remember_roles'] = array(
              2 => '2',
              1 => 0,
              3 => 0,
              4 => 0,
            );
            $handler->display->display_options['filters'][$key . '_tid']['type'] = 'select';
            $handler->display->display_options['filters'][$key . '_tid']['vocabulary'] = $field_info['settings']['allowed_values']['0']['vocabulary'];
            break;

          case 'text_with_summary':
            $handler->display->display_options['filters'][$key . '_value']['id'] = $key . '_value';
            $handler->display->display_options['filters'][$key . '_value']['table'] = 'field_data_' . $key;
            $handler->display->display_options['filters'][$key . '_value']['field'] = $key . '_value';
            $handler->display->display_options['filters'][$key . '_value']['operator'] = 'contains';
            $handler->display->display_options['filters'][$key . '_value']['exposed'] = TRUE;
            $handler->display->display_options['filters'][$key . '_value']['expose']['operator_id'] = $key . '_value_op';
            $handler->display->display_options['filters'][$key . '_value']['expose']['label'] = $val['label'];
            $handler->display->display_options['filters'][$key . '_value']['expose']['operator'] = $key . '_value_op';
            $handler->display->display_options['filters'][$key . '_value']['expose']['identifier'] = $key . '_value';
            $handler->display->display_options['filters'][$key . '_value']['expose']['remember_roles'] = array(
              2 => '2',
              1 => 0,
              3 => 0,
              4 => 0,
            );
            $handler->display->display_options['filters'][$key . '_value']['group_info']['label'] = 'Body (body)';
            $handler->display->display_options['filters'][$key . '_value']['group_info']['identifier'] = $key . '_value';
            $handler->display->display_options['filters'][$key . '_value']['group_info']['remember'] = FALSE;
            $handler->display->display_options['filters'][$key . '_value']['group_info']['group_items'] = array(
              1 => array(),
              2 => array(),
              3 => array(),
            );
            break;
        }
      }
    }

    if (module_exists('locale') && locale_multilingual_node_type($node_type->type) && module_exists('i18n_node')) {
      /* Filter criterion: Content translation: Source translation */
      $handler->display->display_options['filters']['source_translation']['id'] = 'source_translation';
      $handler->display->display_options['filters']['source_translation']['table'] = 'node';
      $handler->display->display_options['filters']['source_translation']['field'] = 'source_translation';
      $handler->display->display_options['filters']['source_translation']['operator'] = '1';
    }

    /* Display: Administer nodes of type $node_type->name */
    $handler = $view->new_display('page', 'Administer ' . $node_type->name, 'page_1');
    $handler->display->display_options['defaults']['filters'] = TRUE;
    $handler->display->display_options['path'] = 'admin/cm/' . $node_type->type;
    $translatables['cm_' . $node_type->type] = array(
      t('Defaults'),
      t("manage {$node_type->type} content"),
      t('Apply'),
      t('Reset'),
      t('Sort by'),
      t('Asc'),
      t('Descending'),
      t('Items per page'),
      t('- All -'),
      t('Offset'),
      t('Bulk operations'),
      t('Title'),
      t('New?'),
      t('Published'),
      t('Translations'),
      t('Operations'),
      t('All'),
      t("Administer {$node_type->type}"),
      t('more'),
    );

    $export[$view->name] = $view;
  }

  return $export;
}
