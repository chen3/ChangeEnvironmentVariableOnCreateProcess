using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows.Input;
using MaterialDesignThemes.Wpf;

namespace Core
{
    public class TreeViewModel : INotifyPropertyChanged
    {
        public TreeViewModel()
        {
            PropertyChanged += (s, e) =>
            {
                if (e.PropertyName != nameof(SelectedItem))
                {
                    return;
                }
                AddCommand.InvokeCanExecuteChanged();
                RemoveSelectedItemCommand.InvokeCanExecuteChanged();
            };

            RemoveSelectedItemCommand = new CommandHandler(parameter =>
            {
                if (parameter is Path path)
                {
                    Paths.Remove(path);
                    return;
                }
                if (!(parameter is Env env))
                {
                    return;
                }
                foreach (Path p in Paths)
                {
                    if (p.Envs.Contains(env) && p.Envs.Remove(env))
                    {
                        break;
                    }
                }
            }, parameter => parameter is Path || parameter is Env);

            AddCommand = new CommandHandler(async parameter =>
            {
                switch (parameter)
                {
                    case null:
                    {
                        var model = new InputDialogModel();
                        var view = new InputDialog("输入路径", "路径")
                        {
                            DataContext = model
                        };
                        var result = await DialogHost.Show(view) as bool?;
                        if (result == true && !string.IsNullOrWhiteSpace(model.Text1))
                            {
                            Paths.Add(new Path
                            {
                                PathFullName = model.Text1
                            });
                        }
                        return;
                    }
                    case Path path:
                    {
                        var model = new InputDialogModel();
                        var view = new InputDialog("输入环境变量", "键", "值")
                        {
                            DataContext = model
                        };
                        var result = await DialogHost.Show(view) as bool?;
                        if (result == true && !string.IsNullOrWhiteSpace(model.Text1) 
                            && !string.IsNullOrWhiteSpace(model.Text2))
                        {
                            path.Envs.Add(new Env
                            {
                                Name = model.Text1,
                                Value = model.Text2
                            });
                        }
                        return;
                    }
                    default:
                    {
                        throw new ApplicationException($"unknow parameter:{parameter.GetType().FullName}:{parameter}");
                    }
                }
            }, parameter => parameter == null || parameter is Path);
        }

        private class InputDialogModel
        {
            public string Text1 { get; set; }
            public string Text2 { get; set; }
        }

        public object SelectedItem { get; set; }
        
        public ObservableCollection<Path> Paths { get; } = new ObservableCollection<Path>();

        public event PropertyChangedEventHandler PropertyChanged;

        public CommandHandler RemoveSelectedItemCommand { get; }

        public CommandHandler AddCommand { get; }
    }

    public class Path : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        public string Icon { get; set; }

        public string PathFullName { get; set; }

        public ObservableCollection<Env> Envs { get; } = new ObservableCollection<Env>();
    }

    public class Env : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        public string Name { get; set; }

        public string Value { get; set; }
    }

    public class CommandHandler : ICommand
    {
        public CommandHandler(Action<object> execute, Func<object, bool> canExecute)
        {
            _execute = execute;
            _canExecute = canExecute;
        }

        public CommandHandler()
        {
            _execute = p => { };
            _canExecute = p => false;
        }

        private readonly Action<object> _execute;

        private readonly Func<object, bool> _canExecute;

        public event EventHandler CanExecuteChanged;

        public void InvokeCanExecuteChanged()
        {
            CanExecuteChanged?.Invoke(this, EventArgs.Empty);
        }

        public bool CanExecute(object parameter)
        {
            return _canExecute(parameter);
        }

        public void Execute(object parameter)
        {
            _execute(parameter);
        }
    }
}
