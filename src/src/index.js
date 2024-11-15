// src/templates/unpopulated-worker/src/index.js
import { html } from "./renderHtml.js";
var src_default = {
  async fetch(request, env) {
    const { DATABASE } = env;
    return new Response(html, {
      headers: {
        "content-type": "text/html; charset=UTF-8"
      }
    });
  }
};
export {
  src_default as default
};
