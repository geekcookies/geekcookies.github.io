---
layout: page
title: Episodi
---

{% assign disqus_thread = "#disqus_thread" %}

{% for post in site.posts %}
  * {{ post.date | date_to_string }} &raquo; [ {{ post.title }} ]({{ post.url }})
{% endfor %}

