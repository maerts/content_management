<?php

namespace Drupal\content_management;

/**
 * @file
 * The permissions for content management module.
 */

use Drupal\Core\DependencyInjection\ContainerInjectionInterface;
use Drupal\Core\Entity\EntityManagerInterface;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Class ContentManagementPermissions.
 */
class ContentManagementPermissions implements ContainerInjectionInterface {
  use StringTranslationTrait;

  /**
   * The entity manager.
   *
   * @var \Drupal\Core\Entity\EntityManagerInterface
   */
  protected $entityManager;

  /**
   * Constructs a ContentManagementPermissions instance.
   *
   * @param \Drupal\Core\Entity\EntityManagerInterface $entityManager
   *   The entity manager.
   */
  public function __construct(EntityManagerInterface $entityManager) {
    $this->entityManager = $entityManager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static($container->get('entity.manager'));
  }

  /**
   * Permissions method for content management.
   *
   * @return array
   *   An array of permissions.
   */
  public function permissions() {
    $permissions = [];
    // Generate permissions for each content type.
    $contentTypes = \Drupal::service('entity.manager')
      ->getStorage('node_type')
      ->loadMultiple();

    foreach ($contentTypes as $contentType) {
      // $contentTypesList[$contentType->id()] = $contentType->label();
      $permissions['content_management_manage_' . $contentType->id()] = [
        'title' => $this->t('Manage the @ct content', ['@ct' => $contentType->label()]),
        'description' => $this->t('Allows a role to manage content of the type @ct, only give this permission to trusted roles', ['@ct' => $contentType->label()]),
      ];
    }
    return $permissions;
  }

}
