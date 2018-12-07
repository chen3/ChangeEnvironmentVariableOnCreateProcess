using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Core
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            this.DataContext = _treeViewModel;
            InitializeComponent();
        }

        private readonly TreeViewModel _treeViewModel = new TreeViewModel();
        
        private void MyTreeView_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            _treeViewModel.SelectedItem = e.NewValue;
        }

        private void MyTreeView_OnMouseDown(object sender, MouseButtonEventArgs e)
        {
            if (MyTreeView.SelectedItem == null || e.ChangedButton != MouseButton.Left)
            {
                return;
            }
            if (!(MyTreeView.ItemContainerGenerator.ContainerFromIndex(0) is TreeViewItem treeViewItem))
            {
                return;
            }
            treeViewItem.IsSelected = true;
            treeViewItem.IsSelected = false;
        }
    }
}
