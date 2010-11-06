//foreach
foreach(const ptr<HeveaElement> &e, m_lstElts) {
 foo(e);
}
foreach(const ptr<HeveaElement> *e, m_lstElts) {
 foo(e);
}
foreach(const ptr<NESTERNAMESPACE::HeveaElement> &e, m_lstElts) {
 foo(e);
}
foreach(str,stringList){ dothis(str); } //foreach

