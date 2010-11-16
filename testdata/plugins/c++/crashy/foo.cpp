/* Source: http://www.kdedevelopers.org/node/3919 */
void ParentWidget::slotDoSomething() {
    SomeDialog dlg( this ); //the dlg-on-stack-variant
    if ( dlg.exec() == QDialog::Accepted ) {
        const QString str = dlg.someUserInput();
        //do something with with str
    }
}
