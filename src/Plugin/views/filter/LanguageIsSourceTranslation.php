<?php

namespace Drupal\content_management\Plugin\views\filter;

use Drupal\views\Plugin\views\filter\FilterPluginBase;

/**
 * Filter rows by equity of two langcodes.
 *
 * The source and target langcodes is used for comparision.
 *
 * @ViewsFilter("language_is_source_translation")
 */
class LanguageIsSourceTranslation extends FilterPluginBase {

  /**
   * {@inheritdoc}
   */
  public function query() {
    $tableAlias = $this->ensureMyTable();
    $this->query->addWhere('AND', "$tableAlias.default_langcode", 1);
  }

}
