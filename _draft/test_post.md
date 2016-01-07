---
layout     : post
title      : "Test post" 
date       : 2016-1-1 13:47:09
categories : podcast
tags       : podcast 
short      : "Test post"
---

Link sponsorizzati:

{% for epdata in site.data.episodes_data %}

### {{ epdata.title }}

 {% for slink in epdata.sponsorlinks %}
 - [{{ slink.text }}]({{ slink.url }})
 {% endfor %}

---
{% endfor %}

