<?php

namespace Drupal\content_management\Access;

/**
 * @file
 * The access for content management module.
 */

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Routing\Access\AccessInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Routing\RouteMatchInterface;
use Symfony\Component\Routing\Route;

/**
 * Checks access for displaying configuration translation page.
 */
class ContentManagementAccessCheck implements AccessInterface {

  /**
   * Checks access.
   *
   * @param \Symfony\Component\Routing\Route $route
   *   The route to check against.
   * @param \Drupal\Core\Routing\RouteMatchInterface $routeMatch
   *   The route match object.
   * @param \Drupal\Core\Session\AccountInterface $account
   *   The currently logged in account.
   *
   * @return \Drupal\Core\Access\AccessResultInterface
   *   The access result.
   *
   * @throws \Drupal\Component\Plugin\Exception\InvalidPluginDefinitionException
   * @throws \Drupal\Component\Plugin\Exception\PluginNotFoundException
   */
  public function access(Route $route, RouteMatchInterface $routeMatch, AccountInterface $account) {
    $contentType = $routeMatch->getParameter('arg_0');

    return ($account->hasPermission('content_management_manage_' . $contentType)) ? AccessResult::allowed()->cachePerPermissions() : AccessResult::forbidden()->cachePerPermissions();
  }

}
