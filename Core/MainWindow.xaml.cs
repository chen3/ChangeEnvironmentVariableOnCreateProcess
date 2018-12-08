using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using mx404.ChangeEnvironmentVariableOnCreateProcess.SharedMemoryManager;

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
        
        private readonly SharedMemoryServerManager _sharedMemoryServer = new SharedMemoryServerManager("MX404ChangeEnvironmentVariableOnCreateProcess");

        private void Sync_OnClick(object sender, RoutedEventArgs e)
        {
            try
            {
                if (_treeViewModel.Paths.Count == 0)
                {
                    return;
                }
                MemData memData = _treeViewModel.ToMemData();
                _sharedMemoryServer.UpdateData(memData);
            }
            catch (Exception ex)
            {
                Snackbar.MessageQueue.Enqueue(ex.Message);
            }
        }
    }

    internal static class TreeViewModelExtend
    {
        internal static MemData ToMemData(this TreeViewModel model)
        {
            var memData = new MemData();
            foreach (Path path in model.Paths)
            {
                if (path.Envs.Count == 0)
                {
                    continue;
                }
                IDictionary<string, string> dictionary = new Dictionary<string, string>();
                foreach (Env env in path.Envs)
                {
                    dictionary.Add(env.Name, env.Value);
                }
                memData.Add(path.PathFullName, dictionary);
            }
            return memData;
        }
    }
}
