using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Xml.Serialization;
using mx404.ChangeEnvironmentVariableOnCreateProcess.SharedMemoryManager;

namespace Core
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    [SuppressMessage("ReSharper", "RedundantExtendsListEntry")]
    [SuppressMessage("ReSharper", "InheritdocConsiderUsage")]
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            TreeViewModel model = null;
            try
            {
                do
                {
                    if (!File.Exists(DataFileFullPath))
                    {
                        break;
                    }
                    string xml = File.ReadAllText(DataFileFullPath);
                    if (string.IsNullOrWhiteSpace(xml))
                    {
                        break;
                    }
                    model = DeserializeModel(xml);
                } while (false);
            }
            catch (Exception)
            {
                model = null;
            }
            _treeViewModel = model ?? new TreeViewModel();
            DataContext = _treeViewModel;
        }

        private readonly TreeViewModel _treeViewModel;
        
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
        
        private readonly SharedMemoryServerManager _sharedMemoryServer =
            new SharedMemoryServerManager("MX404ChangeEnvironmentVariableOnCreateProcess");

        private static readonly string DataFileFullPath =
            System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
                                    "ChangeEnvironmentVariableOnCreateProcess",
                                    "data.xml");

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

                string xml = SerializerModel(_treeViewModel);
                new FileInfo(DataFileFullPath).Directory?.Create();
                File.WriteAllText(DataFileFullPath, xml);
            }
            catch (Exception ex)
            {
                Snackbar.MessageQueue.Enqueue(ex.Message);
            }
        }

        private static string SerializerModel(TreeViewModel model)
        {
            Debug.Assert(model != null);
            using (var memoryStream = new MemoryStream())
            {
                var xmlSerializer = new XmlSerializer(typeof(TreeViewModel));
                xmlSerializer.Serialize(memoryStream, model);
                memoryStream.Position = 0;
                using (var streamReader = new StreamReader(memoryStream))
                {
                    return streamReader.ReadToEnd();
                }
            }
        }

        private static TreeViewModel DeserializeModel(string xml)
        {
            Debug.Assert(!string.IsNullOrWhiteSpace(xml));
            using (var stringReader = new StringReader(xml))
            {
                return (TreeViewModel)new XmlSerializer(typeof(TreeViewModel)).Deserialize(stringReader);
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
