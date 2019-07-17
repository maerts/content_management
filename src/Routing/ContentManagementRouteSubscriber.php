<?php

namespace Drupal\content_management\Routing;

/**
 * @file
 * The permissions route override for content management module.
 */

use Drupal\Core\Routing\RouteSubscriberBase;
use Symfony\Component\Routing\RouteCollection;

/**
 * Listens to the dynamic route events.
 */
class ContentManagementRouteSubscriber extends RouteSubscriberBase {

  /**
   * {@inheritdoc}
   */
  protected function alterRoutes(RouteCollection $collection) {
    if ($route = $collection->get('view.content_management.page_1')) {
      $route->setRequirement('_custom_access', 'Drupal\content_management\Access\ContentManagementAccessCheck::access');
    }
  }

}
