function detectColorScheme(){
  var theme="light";
  var current_theme = localStorage.getItem("ar5iv_theme");
  if(current_theme){
    if(current_theme == "dark"){
      theme = "dark";
    } }
  else if(!window.matchMedia) { return false; }
  else if(window.matchMedia("(prefers-color-scheme: dark)").matches) {
    theme = "dark"; }
  if (theme=="dark") {
    document.documentElement.setAttribute("data-theme", "dark");
  } else {
    document.documentElement.setAttribute("data-theme", "light"); } }

detectColorScheme();

function toggleColorScheme(){
  var current_theme = localStorage.getItem("ar5iv_theme");
  if (current_theme) {
    if (current_theme == "light") {
      localStorage.setItem("ar5iv_theme", "dark"); }
    else {
      localStorage.setItem("ar5iv_theme", "light"); } }
  else {
      localStorage.setItem("ar5iv_theme", "dark"); }
  detectColorScheme();
}

window.onload = function() {
  var canMathML = typeof(MathMLElement) == "function";
  if (!canMathML) {
    var body = document.querySelector("body");
    body.firstElementChild.setAttribute('style', 'opacity: 0;');
    var loading = document.createElement("div");
    loading.setAttribute("id", "mathjax-loading-spinner");
    var message = document.createElement("div");
    message.setAttribute("id", "mathjax-loading-message");
    message.innerText = "Typesetting Equations...";
    body.prepend(loading);
    body.prepend(message);

    var el = document.createElement("script");
    el.src = "https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-mml-chtml.js";
    document.querySelector("head").appendChild(el);

    window.MathJax = {
      startup: {
        pageReady: () => {
          return MathJax.startup.defaultPageReady().then(() => {
            body.removeChild(loading);
            body.removeChild(message);
            body.firstElementChild.removeAttribute('style');
          }); } } };
  }
}

// Auxiliary function, building the preview feature when
// an inline citation is clicked
function clicked_cite(e) {
  e.preventDefault();
  let cite = this.closest('.ltx_cite');
  let next = cite.nextSibling;
  if (next && next.nodeType == Node.ELEMENT_NODE && next.getAttribute('class') == "ar5iv-bibitem-preview") {
    next.remove();
    return; }
  // Before adding a preview modal,
  // cleanup older previews, in case they're still open
  document.querySelectorAll('span.ar5iv-bibitem-preview').forEach(function(node) {
    node.remove();
  })

  // Create the preview
  preview = document.createElement('span');
  preview.setAttribute('class','ar5iv-bibitem-preview');
  let target = document.getElementById(this.getAttribute('href').slice(1));
  target.childNodes.forEach(function (child) {
    preview.append(child.cloneNode(true));
  });
  let close_x = document.createElement('button');
  close_x.setAttribute("aria-label","Close modal for bibliography item preview");
  close_x.textContent = "×";
  close_x.setAttribute('class', 'ar5iv-button-close-preview');
  close_x.setAttribute('onclick','this.parentNode.remove()');
  preview.append(close_x);
  preview.querySelectorAll('.ltx_tag_bibitem').forEach(function(node) {
    node.remove();
  });
  cite.parentNode.insertBefore(preview, cite.nextSibling);
  return;
}
// Global Document initialization:
// - assign the preview feature to all inline citation links
document.querySelectorAll(".ltx_cite .ltx_ref").forEach(function (link) {
  link.addEventListener("click", clicked_cite);
});
