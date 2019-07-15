<?php

namespace Drupal\content_management\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Url;
use Drupal\Core\Link;

/**
 * Provides a Content Management Block.
 *
 * @Block(
 *   id = "content_management_block",
 *   admin_label = @Translation("Content management"),
 * )
 */
class ContentManagementBlock extends BlockBase {

  /**
   * {@inheritdoc}
   */
  public function build() {
    $list = [];

    // Get the current user.
    $user = \Drupal::currentUser();

    $contentTypes = \Drupal::service('entity.manager')
      ->getStorage('node_type')
      ->loadMultiple();

    foreach ($contentTypes as $contentType) {
      if ($user->hasPermission('content_management_manage') || $user->hasPermission('content_management_manage_' . $contentType->id())) {
        $link = Link::fromTextAndUrl($this->t($contentType->label()), Url::fromUri('internal:/admin/content/cm/' . $contentType->id(), []))
          ->toString();
        $list[] = $link;
      }
    }

    $renderArray = [
      '#theme' => 'item_list',
      '#list_type' => 'ul',
      '#items' => $list,
      '#cache' => [
        'contexts' => ['user.roles'],
      ],
    ];
    return $renderArray;
  }

}
