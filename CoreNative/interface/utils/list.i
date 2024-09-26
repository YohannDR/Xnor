%module CoreNative

%typemap(csinterfaces) XnorCore::List<XnorCore::Component*>
%{
    global::System.Collections.Generic.IEnumerable<global::Xnor.Core.Component>
%}

%typemap(cscode) XnorCore::List<XnorCore::Component*>
%{
    private class ListEnumerator : global::System.Collections.Generic.IEnumerator<Component>
    {
        private readonly ComponentList list;
        private uint index;
        private Component current;
        
        public ListEnumerator(ComponentList list)
        {
            this.list = list;
            Reset();
        }
        
        public bool MoveNext()
        {
            if (index >= list.GetSize())
            {
                current = null;
                return false;
            }

            current = list[index++];

            return true;
        }

        public void Reset()
        {
            index = 0;
            current = null;
        }

        public Component Current { get => current; }

        object global::System.Collections.IEnumerator.Current => Current;

        public void Dispose() { }
    }

    public global::System.Collections.Generic.IEnumerator<Component> GetEnumerator()
    {
        return new ListEnumerator(this);
    }

    global::System.Collections.IEnumerator global::System.Collections.IEnumerable.GetEnumerator()
    {
        return GetEnumerator();
    }

    public Component this[uint index]
    {
        get => CoreNative.ComponentListGetItem(this, index);
    }

    public void ForEach(global::System.Action<Component> action)
    {
        foreach (Component c in this)
            action(c);
    }

    public Component Find(global::System.Predicate<Component> predicate)
    {
        foreach (Component c in this)
        {
            if (predicate(c))
                return c;
        }

        return null;
    }

    public bool Exists(global::System.Predicate<Component> predicate)
    {
        foreach (Component c in this)
        {
            if (predicate(c))
                return true;
        }

        return false;
    }
%}

%ignore XnorCore::List<XnorCore::Component*>::Front;
%ignore XnorCore::List<XnorCore::Component*>::Back;

%ignore XnorCore::List<XnorCore::Component*>::Contains;

%ignore XnorCore::List<XnorCore::Component*>::Add();
%ignore XnorCore::List<XnorCore::Component*>::Fill;
%ignore XnorCore::List<XnorCore::Component*>::Insert;
%ignore XnorCore::List<XnorCore::Component*>::RemoveAt;
%ignore XnorCore::List<XnorCore::Component*>::Clear;
%ignore XnorCore::List<XnorCore::Component*>::Resize;

%ignore XnorCore::List<XnorCore::Component*>::List;

%ignore XnorCore::List::AddRange;
%ignore XnorCore::List::RemoveRange;

%ignore XnorCore::List::Iterate;
%ignore XnorCore::List::Exists;
%ignore XnorCore::List::Find;
%ignore XnorCore::List::Sort;

%ignore XnorCore::List::Begin;
%ignore XnorCore::List::End;
%ignore XnorCore::List::CBegin const;
%ignore XnorCore::List::CEnd const;
%ignore XnorCore::List::RBegin;
%ignore XnorCore::List::REnd;
%ignore XnorCore::List::CrBegin const;
%ignore XnorCore::List::CrEnd const;
%ignore XnorCore::List::begin;
%ignore XnorCore::List::end;

%include "utils/list.hpp"

%template(ComponentList) XnorCore::List<XnorCore::Component*>;
