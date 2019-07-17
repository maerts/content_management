<?php

namespace Drupal\content_management\Plugin\Menu\LocalAction;

use Drupal\Core\Menu\LocalActionDefault;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\Core\Routing\RouteProviderInterface;
use Drupal\Core\Routing\RouteMatchInterface;
use Symfony\Component\HttpFoundation\RequestStack;
use Drupal\Core\Entity\EntityManagerInterface;

/**
 * Defines a dynamic link action.
 */
class AddNodeLocalAction extends LocalActionDefault {

  /**
   * The entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityManagerInterface
   */
  protected $entityManager;

  /**
   * The current request.
   *
   * @var Symfony\Component\HttpFoundation\Request
   */
  protected $request;

  /**
   * The constructor.
   *
   * @param array $configuration
   *   The plugin configuration, i.e. an array with configuration values keyed
   *   by configuration option name. The special key 'context' may be used to
   *   initialize the defined contexts by setting it to an array of context
   *   values keyed by context names.
   * @param string $pluginId
   *   The plugin id for the formatter.
   * @param mixed $pluginDefinition
   *   The plugin implementation definition.
   * @param \Drupal\Core\Routing\RouteProviderInterface $routeProvider
   *   The route provider..
   * @param \Symfony\Component\HttpFoundation\RequestStack $requeststack
   *   The request stack.
   * @param \Drupal\Core\Entity\EntityManagerInterface $entityManager
   *   The entity manager.
   */
  public function __construct(array $configuration, $pluginId, $pluginDefinition, RouteProviderInterface $routeProvider, RequestStack $requeststack, EntityManagerInterface $entityManager) {
    parent::__construct($configuration, $pluginId, $pluginDefinition, $routeProvider);

    $this->request = $requeststack->getCurrentRequest();
    $this->entityManager = $entityManager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $pluginId, $pluginDefinition) {
    return new static(
      $configuration,
      $pluginId,
      $pluginDefinition,
      $container->get('router.route_provider'),
      $container->get('request_stack'),
      $container->get('entity.manager')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getRouteParameters(RouteMatchInterface $routeMatch) {
    $parts = explode('admin/content/cm/', $this->request->getPathInfo());
    $contentTypes = $this->entityManager
      ->getStorage('node_type')
      ->loadMultiple();
    if (array_key_exists($parts[1], $contentTypes)) {
      return ['node_type' => $parts[1]];
    }
    return ['node_type' => ''];
  }

}
