<?php

namespace Drupal\content_management\Form;

use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Entity\EntityManagerInterface;
use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Configure Content Management settings for this site.
 */
class ContentManagementManageForm extends ConfigFormBase {

  /**
   * The allowed field types.
   *
   * @var array
   */
  protected const ALLOWED_TYPES = ['integer', 'string', 'boolean'];

  /**
   * The locked fields.
   *
   * @var array
   */
  protected const LOCKED_FIELDS = ['title'];

  /**
   * The Entity manager.
   *
   * @var \Drupal\Core\Entity\EntityManager
   */
  private $entityManager;

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('config.factory'),
      $container->get('entity.manager')
    );
  }

  /**
   * ContentManagementManageForm constructor.
   *
   * @param \Drupal\Core\Config\ConfigFactoryInterface $configFactory
   *   The config factory.
   * @param \Drupal\Core\Entity\EntityManagerInterface $entityManager
   *   The entity manager to query content types.
   */
  public function __construct(ConfigFactoryInterface $configFactory, EntityManagerInterface $entityManager) {
    parent::__construct($configFactory);

    $this->entityManager = $entityManager;
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'content_management_manage';
  }

  /**
   * {@inheritdoc}
   */
  protected function getEditableConfigNames() {
    return ['content_management.admin_settings'];
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->config('content_management.admin_settings');

    $form['intro'] = [
      '#markup' => $this->t('This is a configuration form to enable additional fields, if possible, to be display in the content management views.'),
    ];

    $contentTypes = $this->entityManager
      ->getStorage('node_type')
      ->loadMultiple();

    foreach ($contentTypes as $contentType) {
      $form[$contentType->id()] = [
        '#type' => 'details',
        '#title' => $contentType->label(),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      ];
      $fields = $this->entityManager->getFieldDefinitions('node', $contentType->id());
      $settings = is_null($config->get($contentType->id())) ? [] : $config->get($contentType->id());
      foreach ($fields as $field) {
        if (in_array($field->getType(), $this::ALLOWED_TYPES, TRUE) && !in_array($field->getName(), $this::LOCKED_FIELDS, TRUE)) {
          $key = $contentType->id() . '_' . $field->getName();
          $form[$contentType->id()][$key] = [
            '#type' => 'checkbox',
            '#title' => $field->getLabel(),
            '#default_value' => isset($settings[$field->getName()]) ? $settings[$field->getName()] : 0,
          ];
        }
      }
    }

    return parent::buildForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $config = $this->config('content_management.admin_settings');
    $form_state->cleanValues();

    $contentTypes = $this->entityManager
      ->getStorage('node_type')
      ->loadMultiple();

    foreach ($contentTypes as $contentType) {
      $fields = $this->entityManager->getFieldDefinitions('node', $contentType->id());
      $store = [];
      foreach ($fields as $field) {
        if (in_array($field->getType(), $this::ALLOWED_TYPES, TRUE) && !in_array($field->getName(), $this::LOCKED_FIELDS, TRUE) && $form_state->hasValue($contentType->id() . '_' . $field->getName())) {
          $store[$field->getName()] = $form_state->getvalue($contentType->id() . '_' . $field->getName());
        }
      }
      $config->set($contentType->id(), $store);
    }

    $config->save();

    parent::submitForm($form, $form_state);
  }

}
