<?php

namespace Drupal\content_management\Plugin\Derivative;

/**
 * @file
 * Contains \Drupal\content_management\Plugin\Derivative\ContentMenuLink.
 */

use Drupal\Component\Plugin\Derivative\DeriverBase;
use Drupal\Core\Plugin\Discovery\ContainerDeriverInterface;
use Drupal\Core\Entity\EntityManagerInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Derivative class that provides the menu links for the Products.
 */
class ContentMenuLink extends DeriverBase implements ContainerDeriverInterface {

  /**
   * The entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityManagerInterface
   */
  protected $entityManager;

  /**
   * Creates a ProductMenuLink instance.
   *
   * @param string $basePluginId
   *   The plugin base.
   * @param \Drupal\Core\Entity\EntityManagerInterface $entityManager
   *   The entity manager.
   */
  public function __construct($basePluginId, EntityManagerInterface $entityManager) {
    $this->entityManager = $entityManager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, $basePluginId) {
    return new static(
      $basePluginId,
      $container->get('entity.manager')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getDerivativeDefinitions($basePluginDefinition) {
    $links = [];

    $contentTypes = $this->entityManager
      ->getStorage('node_type')
      ->loadMultiple();

    foreach ($contentTypes as $id => $ct) {
      // Direct link to content overview pages.
      $links['content_management.' . $id] = [
        'title' => $ct->label(),
        'route_name' => 'view.content_management.page_1',
        'route_parameters' => ['arg_0' => $ct->id()],
        'parent' => 'system.admin_content',
        'menu_name' => 'admin',
      ] + $basePluginDefinition;
    }

    return $links;
  }

}
