(function ($) {
    $(function(){

        // do not show in frames
        if (self !== top) {
            return;
        }

        if (typeof Drupal.settings.admin_menu == 'undefined' || !Drupal.settings.admin_menu.suppress) {
            var anchor = document.createElement('A');
            anchor.href = Drupal.settings.content_management.admin_path;
            anchor.appendChild(document.createTextNode('Manage'));
            anchor.style.position = 'absolute';
            anchor.style.left = '0px';
            anchor.style.top = '30px';
            anchor.style.display = 'block';
            anchor.style.height = '59px';
            anchor.style.width = '22px';
            anchor.style.overflow = 'hidden';
            anchor.style.textIndent = '-999em';
            anchor.style.background = 'transparent url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABYAAAA7CAYAAAB/hnrTAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAyJpVFh0WE1MOmNvbS5hZG9iZS54bXAAAAAAADw/eHBhY2tldCBiZWdpbj0i77u/IiBpZD0iVzVNME1wQ2VoaUh6cmVTek5UY3prYzlkIj8+IDx4OnhtcG1ldGEgeG1sbnM6eD0iYWRvYmU6bnM6bWV0YS8iIHg6eG1wdGs9IkFkb2JlIFhNUCBDb3JlIDUuMC1jMDYwIDYxLjEzNDc3NywgMjAxMC8wMi8xMi0xNzozMjowMCAgICAgICAgIj4gPHJkZjpSREYgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIj4gPHJkZjpEZXNjcmlwdGlvbiByZGY6YWJvdXQ9IiIgeG1sbnM6eG1wPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvIiB4bWxuczp4bXBNTT0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL21tLyIgeG1sbnM6c3RSZWY9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9zVHlwZS9SZXNvdXJjZVJlZiMiIHhtcDpDcmVhdG9yVG9vbD0iQWRvYmUgUGhvdG9zaG9wIENTNSBNYWNpbnRvc2giIHhtcE1NOkluc3RhbmNlSUQ9InhtcC5paWQ6NzQzNzVDNDJDMzc5MTFERjg2OUU4MDE3MjYyNDg3MTQiIHhtcE1NOkRvY3VtZW50SUQ9InhtcC5kaWQ6NzQzNzVDNDNDMzc5MTFERjg2OUU4MDE3MjYyNDg3MTQiPiA8eG1wTU06RGVyaXZlZEZyb20gc3RSZWY6aW5zdGFuY2VJRD0ieG1wLmlpZDo3NDM3NUM0MEMzNzkxMURGODY5RTgwMTcyNjI0ODcxNCIgc3RSZWY6ZG9jdW1lbnRJRD0ieG1wLmRpZDo3NDM3NUM0MUMzNzkxMURGODY5RTgwMTcyNjI0ODcxNCIvPiA8L3JkZjpEZXNjcmlwdGlvbj4gPC9yZGY6UkRGPiA8L3g6eG1wbWV0YT4gPD94cGFja2V0IGVuZD0iciI/PpcpIgIAAAM0SURBVHja7JhPSJNxGMe/migu58EtjGmOIDf1sg5lKL7LzEsURhB0iPLQYRU0wubBS+SpS1nQwXboIMGgiOwQdNLBDKycoAiLzaj1Z4u3fCXd0BpB/Z53vXNzvnv3vm6B4A8eeNlv7+f3vM//9y3hOO4PCrdeMhn2+XyeMhR2dZDY7XaUojjrUrHAHcUCY+uBZaPCd3tC8eYHz2sx4rWoA8+Gsn9rrgfKdZvU+IqbS12fOvQRXfs/paAvJoGx2Ra8mTeoB1frfjPge5xs42Fk9ydWgEdjVXj2qgmRxUrtGj8dmExpSNDRySrEV3ei2/Y59Z+3XwyyWsuC021J12eOxtlVfJ3zoB7sut+i+LgRQafeFLkcs6mouOdQjuPxmQaMvjarA9ssylr5Q7/Ua2y/xhXHFK2NQl7Ok4tpWfCti4Hi1AppLTDFp1jdmJrfh+WV8rzDbYfZbL6x0caPJSbLS6gzJB152LaIxtrvbGcV8Z+VmA3XYGG5QhZckk+XrqtZFVPZWs+j1bKWlblMUZoPtLXxWxb0AysZkUW9+qgYOD2BgwxkNKyBqBAFI7tZVu5i9i7TFm7H2jKdFxVq/z1BjJXTmPbqJi3SOHkIv0G4aahuRcu8dMdV6zJrQi4TKIK7bV9x/fw7mV1edGbvXU49+PLxJPTO4wYWVlUZe+e6AmLSUJNVXTbJaTQCbHxjCwMHskyUv40ZvPdI9oBhMvCbcx5pfeEEX9io+O/NlBpAc72AAxaetSaN9Zimoaa6pRRofR8cn6lQr/HI1Qns3ZNZ8Cl26Tea3W4+4bTZWIJSyD0cTw6AZAZqWVJB0gQmID16UgLi/BYR0sMt93yg2EHSnbXexpSVqjMvPTpIJO9LB1FHyZV5JQV+M93Cb03b4PzAVqsVQ0ND6OnpgV6vx+DgIBwOR15g2dmNliAIMBqNcDqdmJ6eRigUQn9/PxKJBObm5rSDaRGgvb1d1Nrj8YiHkdbhcFgUzTYmc5hMJni9XsRiMXR2diIYDMLv92tvTWRXl8uFaDQKt9stakqH9PX1iYdoLkIEISENJe3pEBKltV0rUt/figUeLjSYvhSepS+FfwUYAK/bRZgTSN01AAAAAElFTkSuQmCC) no-repeat';
            anchor.style.zIndex = '200';
            document.body.insertBefore(anchor, document.body.firstChild);
        }
    })
}(jQuery));
