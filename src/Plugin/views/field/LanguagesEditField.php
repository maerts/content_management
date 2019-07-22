<?php

namespace Drupal\content_management\Plugin\views\field;

use Drupal\Core\Language\LanguageManager;
use Drupal\Core\Link;
use Drupal\Core\Url;
use Drupal\views\Plugin\views\field\FieldPluginBase;
use Drupal\views\ResultRow;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides Languages edit field field handler.
 *
 * @ViewsField("languages_edit_field")
 */
class LanguagesEditField extends FieldPluginBase {

  /**
   * The language manager.
   *
   * @var \Drupal\Core\Language\LanguageManager
   */
  protected $languageManager;

  /**
   * Constructs a new LanguagesEditField instance.
   *
   * @param array $configuration
   *   The plugin configuration, i.e. an array with configuration values keyed
   *   by configuration option name. The special key 'context' may be used to
   *   initialize the defined contexts by setting it to an array of context
   *   values keyed by context names.
   * @param string $pluginId
   *   The plugin_id for the plugin instance.
   * @param mixed $pluginDefinition
   *   The plugin implementation definition.
   * @param \Drupal\Core\Language\LanguageManager $languageManager
   *   The language manager.
   */
  public function __construct(array $configuration, $pluginId, $pluginDefinition, LanguageManager $languageManager) {
    parent::__construct($configuration, $pluginId, $pluginDefinition);
    $this->languageManager = $languageManager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $pluginId, $pluginDefinition) {
    return new static(
      $configuration,
      $pluginId,
      $pluginDefinition,
      $container->get('language_manager')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function query() {
    // Leave empty to avoid a query on this field.
  }

  /**
   * {@inheritdoc}
   */
  protected function defineOptions() {
    $options = parent::defineOptions();
    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function render(ResultRow $values) {
    $currentPath = \Drupal::service('path.current')->getPath();
    $currentLanguage = $this->languageManager->getCurrentLanguage();
    $entity = $this->getEntity($values);
    // Set a default language, this should never be used.
    $langDefault = 'und';
    foreach ($entity->getTranslationLanguages() as $code => $lang) {
      if ($entity->getTranslation($code)->isDefaultTranslation()) {
        $langDefault = $code;
      }
    }

    $eid = $entity->id();

    $value = [];
    if ($entity->isTranslatable()) {
      foreach ($this->languageManager->getLanguages() as $code => $lang) {
        $link = Link::fromTextAndUrl($code, Url::fromUri('internal:/node/' . $eid . '/translations/add/' . $langDefault . '/' . $code, ['language' => $lang, 'query' => ['destination' => $currentPath]]))->toRenderable();
        $link['#attributes'] = ['class' => 'translate', 'title' => $this->t('Create new translation')];
        $mark = 0;

        if ($entity->hasTranslation($code)) {
          $tEntity = $entity->getTranslation($code);
          $mark = node_mark($tEntity->id(), $tEntity->getRevisionCreationTime());
          $link = Link::fromTextAndUrl($code, Url::fromUri('internal:/node/' . $eid . '/edit', ['language' => $lang, 'query' => ['destination' => $currentPath]]))->toRenderable();
          $link['#attributes'] = ['class' => $tEntity->isPublished() ? 'published' : 'unpublished', 'title' => $tEntity->isPublished() ? $this->t('Published') : $this->t('unpublished')];
        }
        $value[] = render($link) . $this->markNode($mark);
      }
    }
    else {
      $tEntity = $entity->getTranslation($langDefault);
      $mark = node_mark($tEntity->id(), $tEntity->getRevisionCreationTime());
      $link = Link::fromTextAndUrl($langDefault, Url::fromUri('internal:/node/' . $eid . '/edit', ['language' => $currentLanguage, 'query' => ['destination' => $currentPath]]))->toRenderable();
      $link['#attributes'] = ['class' => $tEntity->isPublished() ? 'published' : 'unpublished', 'title' => $tEntity->isPublished() ? $this->t('Published') : $this->t('unpublished')];
      $value[] = render($link) . $this->markNode($mark);
    }

    return ['#markup' => implode(', ', $value)];
  }

  /**
   * Helper function to get the node status.
   *
   * @param int $type
   *   Integer reflecting the nodes' status.
   *
   * @return string
   *   Indicating whether the node is new or recently updated.
   */
  private function markNode($type) {
    if ($type === MARK_NEW) {
      return '<span class="marker">*</span>';
    }
    elseif ($type === MARK_UPDATED) {
      return '<span class="marker">!</span>';
    }
    return '';
  }

}
